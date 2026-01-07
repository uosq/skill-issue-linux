#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

#include "../imgui/imgui_impl_vulkan.h"
#include "../imgui/imgui_impl_sdl2.h"

#include "../sdk/interfaces/interfaces.h"
#include "../settings.h"

#include "sdl.h"
#include "../libdetour/libdetour.h"

#include <fstream>

/*
This is not working
For now, we gotta start TF2 with OpenGL
*/

static VkDevice vk_device;
static VkAllocationCallbacks* vk_allocator = NULL;
static VkQueueFamilyProperties* queue_families;
static uint32_t queue_family = (uint32_t)-1;
static VkRenderPass vk_render_pass = VK_NULL_HANDLE;
static VkDescriptorPool vk_descriptor_pool = VK_NULL_HANDLE;
static VkExtent2D vk_image_extent = { };
static VkPhysicalDevice vk_physical_device = VK_NULL_HANDLE;
static VkInstance vk_instance = VK_NULL_HANDLE;
static VkPipelineCache vk_pipeline_cache = VK_NULL_HANDLE;
static uint32_t min_image_count = 3;
static ImGui_ImplVulkanH_Frame g_Frames[8] = { };
static ImGui_ImplVulkanH_FrameSemaphores g_FrameSemaphores[8] = { };
static uint32_t count;

using QueuePresentFn = VkResult(*)(VkQueue, const VkPresentInfoKHR*);
static QueuePresentFn original_QueuePresent = nullptr;
//DETOUR_DECL_TYPE(VkResult, original_QueuePresent, VkQueue, const VkPresentInfoKHR*);

using CreateSwapchainFn = VkResult(*)(VkDevice, const VkSwapchainCreateInfoKHR*, const VkAllocationCallbacks*, VkSwapchainKHR*);
static CreateSwapchainFn original_CreateSwapchain = nullptr;
//DETOUR_DECL_TYPE(VkResult, original_CreateSwapchain, VkDevice, const VkSwapchainCreateInfoKHR*, const VkAllocationCallbacks*, VkSwapchainKHR*);

using AcquireNextImageFn = VkResult(*)(VkDevice, VkSwapchainKHR, uint64_t, VkSemaphore, VkFence, uint32_t*);
static AcquireNextImageFn original_AcquireNextImage = nullptr;
//DETOUR_DECL_TYPE(VkResult, original_AcquireNextImage, VkDevice, VkSwapchainKHR, uint64_t, VkSemaphore, VkFence, uint32_t*);

using AcquireNextImage2Fn = VkResult(*)(VkDevice, const VkAcquireNextImageInfoKHR*,  uint32_t*);
static AcquireNextImage2Fn original_AcquireNextImage2 = nullptr;
//DETOUR_DECL_TYPE(VkResult, original_AcquireNextImage2, VkDevice, const VkAcquireNextImageInfoKHR*,  uint32_t*);

using vkGetDeviceProcAddrFn = PFN_vkVoidFunction(*)(VkDevice, const char*);
static vkGetDeviceProcAddrFn original_vkGetDeviceProcAddr = nullptr;
DETOUR_DECL_TYPE(PFN_vkVoidFunction, original_vkGetDeviceProcAddr, VkDevice, const char*);

/*static detour_ctx_t queuepresent_detour;
static detour_ctx_t swapchain_detour;
static detour_ctx_t nextimage_detour;
static detour_ctx_t nextimage2_detour;*/

static detour_ctx_t getdeviceproc_detour;

static VkResult Hooked_QueuePresent(VkQueue queue, const VkPresentInfoKHR* present_info)
{
	if (!vk_device) {
		//VkResult result;
		//DETOUR_ORIG_GET(&queuepresent_detour, result, original_QueuePresent, queue, present_info);
		return original_QueuePresent(queue, present_info);
	}

	// We havn't gotten SDL's window context yet
	if (tfwindow == nullptr) {
		//VkResult result;
		//DETOUR_ORIG_GET(&queuepresent_detour, result, original_QueuePresent, queue, present_info);
		return original_QueuePresent(queue, present_info);
	}

	if (ImGui::GetCurrentContext() == nullptr) {
		ImGui::CreateContext();
		ImGui_ImplSDL2_InitForVulkan(tfwindow);
	}

	// https://github.com/bruhmoment21/UniversalHookX/blob/main/UniversalHookX/src/hooks/backend/vulkan/hook_vulkan.cpp#L418
	VkQueue graphicQueue = VK_NULL_HANDLE;  
	for (uint32_t i = 0; i < count; ++i)
	{
		const VkQueueFamilyProperties& family = queue_families[i];
		for (uint32_t j = 0; j < family.queueCount; ++j)
		{
			VkQueue it = VK_NULL_HANDLE;
			vkGetDeviceQueue(vk_device, i, j, &it);

			if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {     	
				if (graphicQueue == VK_NULL_HANDLE) {
					graphicQueue = it;
				}
			}
		}
	}
  
	for (uint32_t i = 0; i < present_info->swapchainCount; ++i)
	{
		VkSwapchainKHR swapchain = present_info->pSwapchains[i];
		if (g_Frames[0].Framebuffer == VK_NULL_HANDLE)
		{
			// begin creating render target
			uint32_t uImageCount;
			vkGetSwapchainImagesKHR(vk_device, swapchain, &uImageCount, NULL);

			VkImage backbuffers[8] = { };
			vkGetSwapchainImagesKHR(vk_device, swapchain, &uImageCount, backbuffers);
			
			for (uint32_t i = 0; i < uImageCount; ++i) {
				g_Frames[i].Backbuffer = backbuffers[i];

				ImGui_ImplVulkanH_Frame* fd = &g_Frames[i];
				ImGui_ImplVulkanH_FrameSemaphores* fsd = &g_FrameSemaphores[i];
				{
					VkCommandPoolCreateInfo info = { };
					info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
					info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
					info.queueFamilyIndex = queue_family;

					vkCreateCommandPool(vk_device, &info, vk_allocator, &fd->CommandPool);
				}
				{
					VkCommandBufferAllocateInfo info = { };
					info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
					info.commandPool = fd->CommandPool;
					info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
					info.commandBufferCount = 1;

					vkAllocateCommandBuffers(vk_device, &info, &fd->CommandBuffer);
				}
				{
					VkFenceCreateInfo info = { };
					info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
					info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
					vkCreateFence(vk_device, &info, vk_allocator, &fd->Fence);
				}

				{
					VkSemaphoreCreateInfo info = { };
					info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
					vkCreateSemaphore(vk_device, &info, vk_allocator, &fsd->ImageAcquiredSemaphore);
					vkCreateSemaphore(vk_device, &info, vk_allocator, &fsd->RenderCompleteSemaphore);
				}
			}

			// Create the Render Pass
			{
				VkAttachmentDescription attachment = { };
				attachment.format = VK_FORMAT_B8G8R8A8_UNORM;
				attachment.samples = VK_SAMPLE_COUNT_1_BIT;
				attachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

				VkAttachmentReference color_attachment = { };
				color_attachment.attachment = 0;
				color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

				VkSubpassDescription subpass = { };
				subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
				subpass.colorAttachmentCount = 1;
				subpass.pColorAttachments = &color_attachment;

				VkRenderPassCreateInfo info = { };
				info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
				info.attachmentCount = 1;
				info.pAttachments = &attachment;
				info.subpassCount = 1;
				info.pSubpasses = &subpass;

				if (vkCreateRenderPass(vk_device, &info, vk_allocator, &vk_render_pass) != VK_SUCCESS)
				{
					interfaces::Cvar->ConsolePrintf("renderpass err\n");
					//VkResult result;
					//DETOUR_ORIG_GET(&queuepresent_detour, result, original_QueuePresent, queue, present_info);
					return original_QueuePresent(queue, present_info);
				}
			}

			// Create The Image Views
			{
				VkImageViewCreateInfo info = { };
				info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				info.viewType = VK_IMAGE_VIEW_TYPE_2D;
				info.format = VK_FORMAT_B8G8R8A8_UNORM;

				info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				info.subresourceRange.baseMipLevel = 0;
				info.subresourceRange.levelCount = 1;
				info.subresourceRange.baseArrayLayer = 0;
				info.subresourceRange.layerCount = 1;

				for (uint32_t i = 0; i < uImageCount; ++i)
				{
					ImGui_ImplVulkanH_Frame* fd = &g_Frames[i];
					info.image = fd->Backbuffer;

					vkCreateImageView(vk_device, &info, vk_allocator, &fd->BackbufferView);
				}
			}
			
			// Create Framebuffer
			{
				VkImageView attachment[1];
				VkFramebufferCreateInfo info = { };
				info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
				info.renderPass = vk_render_pass;
				info.attachmentCount = 1;
				info.pAttachments = attachment;
				info.layers = 1;

				for (uint32_t i = 0; i < uImageCount; ++i)
				{
					ImGui_ImplVulkanH_Frame* fd = &g_Frames[i];
					attachment[0] = fd->BackbufferView;

					if (vkCreateFramebuffer(vk_device, &info, vk_allocator, &fd->Framebuffer) != VK_SUCCESS)
					{
						interfaces::Cvar->ConsolePrintf("framebuffer err\n");
						//VkResult result;
						//DETOUR_ORIG_GET(&queuepresent_detour, result, original_QueuePresent, queue, present_info);
						return original_QueuePresent(queue, present_info);
					}
				}
			}

			if (!vk_descriptor_pool) // Create Descriptor Pool.
			{
				constexpr VkDescriptorPoolSize pool_sizes[] =
				{
					{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
					{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
					{VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
					{VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
					{VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
					{VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
					{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
					{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
					{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
					{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
					{VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}
				};

				VkDescriptorPoolCreateInfo pool_info = { };
				pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
				pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
				pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
				pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
				pool_info.pPoolSizes = pool_sizes;

				vkCreateDescriptorPool(vk_device, &pool_info, vk_allocator, &vk_descriptor_pool);
			}
				//end creating render target
		}
		
		ImGui_ImplVulkanH_Frame* fd = &g_Frames[present_info->pImageIndices[i]];
		ImGui_ImplVulkanH_FrameSemaphores* fsd = &g_FrameSemaphores[present_info->pImageIndices[i]];
		{
			vkWaitForFences(vk_device, 1, &fd->Fence, VK_TRUE, ~0ull);
		}

		{
			vkResetCommandBuffer(fd->CommandBuffer, 0);

			VkCommandBufferBeginInfo info = { };
			info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			vkBeginCommandBuffer(fd->CommandBuffer, &info);
		}
		
		{
			VkRenderPassBeginInfo info = { };
			info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			info.renderPass = vk_render_pass;
			info.framebuffer = fd->Framebuffer;
			if (vk_image_extent.width == 0 || vk_image_extent.height == 0) {
				int resx, resy;
				interfaces::Engine->GetScreenSize(resx, resy);

				info.renderArea.extent.width = resx;
				info.renderArea.extent.height = resy;
			} else {
				info.renderArea.extent = vk_image_extent;
			}

			vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
		}
		
		if (!ImGui::GetIO().BackendRendererUserData)
		{
			ImGui_ImplVulkan_InitInfo init_info = { };
			init_info.Instance = vk_instance;
			init_info.PhysicalDevice = vk_physical_device;
			init_info.Device = vk_device;
			init_info.QueueFamily = queue_family;
			init_info.Queue = graphicQueue;
			init_info.PipelineCache = vk_pipeline_cache;
			init_info.DescriptorPool = vk_descriptor_pool;
			init_info.MinImageCount = min_image_count;
			init_info.ImageCount = min_image_count;
			init_info.Allocator = vk_allocator;
			
			int resx, resy;
			interfaces::Engine->GetScreenSize(resx, resy);
			
			ImGui::GetIO().DisplaySize.x = resx;
			ImGui::GetIO().DisplaySize.y = resy;
			
			ImGui_ImplVulkan_Init(&init_info);
		}

		if (ImGui::IsKeyPressed(ImGuiKey_Insert, false) || ImGui::IsKeyPressed(ImGuiKey_F11, false))
		{
			settings.menu_open = !settings.menu_open;
			interfaces::Surface->SetCursorAlwaysVisible(settings.menu_open);
		}

		// begin drawing our stuff
		ImGui_ImplVulkan_NewFrame( );
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame( );
			
		if (settings.menu_open)
			DrawMainWindow();
		
		ImGui::Render();
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), fd->CommandBuffer);
		// stop drawing our stuff
		
		vkCmdEndRenderPass(fd->CommandBuffer);
		vkEndCommandBuffer(fd->CommandBuffer);

		constexpr VkPipelineStageFlags stages_wait = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
		{
			VkSubmitInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			info.pWaitDstStageMask = &stages_wait;

			info.signalSemaphoreCount = 1;
			info.pSignalSemaphores = &fsd->RenderCompleteSemaphore;

			vkQueueSubmit(queue, 1, &info, VK_NULL_HANDLE);
		}

		{
			VkSubmitInfo info = { };
			info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			info.commandBufferCount = 1;
			info.pCommandBuffers = &fd->CommandBuffer;

			info.pWaitDstStageMask = &stages_wait;
			info.waitSemaphoreCount = 1;
			info.pWaitSemaphores = &fsd->RenderCompleteSemaphore;

			info.signalSemaphoreCount = 1;
			info.pSignalSemaphores = &fsd->ImageAcquiredSemaphore;

			vkQueueSubmit(graphicQueue, 1, &info, fd->Fence);
		}
	}

	//VkResult result;
	//DETOUR_ORIG_GET(&queuepresent_detour, result, original_QueuePresent, queue, present_info);
	return original_QueuePresent(queue, present_info);
}

static VkResult Hooked_CreateSwapchain(VkDevice device, const VkSwapchainCreateInfoKHR* create_info, const VkAllocationCallbacks* allocator, VkSwapchainKHR* swapchain)
{
	for (uint32_t i = 0; i < 8; ++i)
	{
		if (g_Frames[i].Fence)
		{
			vkDestroyFence(vk_device, g_Frames[i].Fence, vk_allocator);
			g_Frames[i].Fence = VK_NULL_HANDLE;
		}
		if (g_Frames[i].CommandBuffer)
		{
			vkFreeCommandBuffers(vk_device, g_Frames[i].CommandPool, 1, &g_Frames[i].CommandBuffer);
			g_Frames[i].CommandBuffer = VK_NULL_HANDLE;
		}
		if (g_Frames[i].CommandPool)
		{
			vkDestroyCommandPool(vk_device, g_Frames[i].CommandPool, vk_allocator);
			g_Frames[i].CommandPool = VK_NULL_HANDLE;
		}
		if (g_Frames[i].BackbufferView)
		{
			vkDestroyImageView(vk_device, g_Frames[i].BackbufferView, vk_allocator);
			g_Frames[i].BackbufferView = VK_NULL_HANDLE;
		}
		if (g_Frames[i].Framebuffer)
		{
			vkDestroyFramebuffer(vk_device, g_Frames[i].Framebuffer, vk_allocator);
			g_Frames[i].Framebuffer = VK_NULL_HANDLE;
		}
	}

	for (uint32_t i = 0; i < 8; ++i)
	{
		if (g_FrameSemaphores[i].ImageAcquiredSemaphore)
		{
			vkDestroySemaphore(vk_device, g_FrameSemaphores[i].ImageAcquiredSemaphore, vk_allocator);
			g_FrameSemaphores[i].ImageAcquiredSemaphore = VK_NULL_HANDLE;
		}
		if (g_FrameSemaphores[i].RenderCompleteSemaphore)
		{
			vkDestroySemaphore(vk_device, g_FrameSemaphores[i].RenderCompleteSemaphore, vk_allocator);
			g_FrameSemaphores[i].RenderCompleteSemaphore = VK_NULL_HANDLE;
		}
	}
	
	vk_image_extent = create_info->imageExtent;

	//VkResult result;
	//DETOUR_ORIG_GET(&swapchain_detour, result, original_CreateSwapchain, device, create_info, allocator, swapchain);
	return original_CreateSwapchain(device, create_info, allocator, swapchain);
}

static VkResult Hooked_AcquireNextImage(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* image_index)
{
	vk_device = device;
	// VkResult result;
	//DETOUR_ORIG_GET(&nextimage_detour, result, original_AcquireNextImage, device, swapchain, timeout, semaphore, fence, image_index);
	return original_AcquireNextImage(device, swapchain, timeout, semaphore, fence, image_index);
}

static VkResult Hooked_AcquireNextImage2(VkDevice device, const VkAcquireNextImageInfoKHR* acquire_info, uint32_t* image_index)
{
	vk_device = device; 
	//VkResult result;
	//DETOUR_ORIG_GET(&nextimage2_detour, result, original_AcquireNextImage2, device, acquire_info, image_index);
	return original_AcquireNextImage2(device, acquire_info, image_index);
}

inline PFN_vkVoidFunction Hooked_vkGetDeviceProcAddr(VkDevice device, const char* name)
{
    //void* fn = original_vkGetDeviceProcAddr(device, name);
    PFN_vkVoidFunction fn;
    DETOUR_ORIG_GET(&getdeviceproc_detour, fn, original_vkGetDeviceProcAddr, device, name);

    if (!strcmp(name, "vkQueuePresentKHR"))
    {
	if (original_QueuePresent == nullptr)
		original_QueuePresent = (QueuePresentFn)fn;
	return (PFN_vkVoidFunction)&Hooked_QueuePresent;
    }

    if (!strcmp(name, "vkAcquireNextImageKHR"))
    {
	if (original_AcquireNextImage == nullptr)
		original_AcquireNextImage = (AcquireNextImageFn)fn;
        return (PFN_vkVoidFunction)&Hooked_AcquireNextImage;
    }

    if (!strcmp(name, "vkAcquireNextImage2KHR"))
    {
	if (original_AcquireNextImage2 == nullptr)
		original_AcquireNextImage2 = (AcquireNextImage2Fn)fn;
        return (PFN_vkVoidFunction)&Hooked_AcquireNextImage2;
    }

    if (!strcmp(name, "vkCreateSwapchainKHR"))
    {
	if (original_CreateSwapchain == nullptr)
		original_CreateSwapchain = (CreateSwapchainFn)fn;
        return (PFN_vkVoidFunction)&Hooked_CreateSwapchain;
    }

    return fn;
}

static void* GetModuleBaseAddress(std::string module_name) {
	std::ifstream file;
	file.open("/proc/self/maps");

	bool found = false;
	
	std::string line;
	std::string concat_line;
	while (std::getline(file, line))
	{
		for (int i = 0; i < line.length(); i++)
		{
			concat_line = "";

			for (int h = 0; i+h < line.length() && h < module_name.length(); h++)
			{
				concat_line += line[i+h];
			}

			if (concat_line == module_name) { found = true; break; }
		}

		if (found == true) {break;}
	}

	concat_line = "";
	for (int i = 0; i <= line.length(); i++)
	{
		if (line[i] == '-') { break; }
		concat_line += line[i];
	}

	std::stringstream ss;
	ss << std::hex << "0x"+concat_line;

	unsigned long result;
	ss >> result;

	return (void*)result;  
}

// https://github.com/bruhmoment21/UniversalHookX/blob/main/UniversalHookX/src/hooks/backend/vulkan/hook_vulkan.cpp
inline void HookVulkan()
{
	void *dxvk_d3d9 = GetModuleBaseAddress("libdxvk_d3d9.so");
	if (dxvk_d3d9 == nullptr)
		return;

	void* vulkan = dlopen("libvulkan.so.1", RTLD_LAZY | RTLD_NOLOAD);
	if (vulkan == nullptr)
		return interfaces::Cvar->ConsolePrintf("Couldn't load vulkan!\n");

	original_vkGetDeviceProcAddr = (vkGetDeviceProcAddrFn)dlsym(vulkan, "vkGetDeviceProcAddr");
	if (original_vkGetDeviceProcAddr == nullptr)
		return interfaces::Cvar->ConsolePrintf("Failed to get vkGetDeviceProcAddr\n");

	detour_init(&getdeviceproc_detour, (void*)original_vkGetDeviceProcAddr, (void*)&Hooked_vkGetDeviceProcAddr);
	if (!detour_enable(&getdeviceproc_detour))
		return interfaces::Cvar->ConsolePrintf("Failed to hook vkGetDeviceProcAddr\n");

	interfaces::Cvar->ConsolePrintf("Hooked Vulkan\n");
}