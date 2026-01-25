# Render Library

## Methods

### > GetColorModulation( )
returns red: **number**, green: **number**, blue: **number**

### > SetColorModulation( red: **number**, green: **number**, blue: **number** )
The numbers must be between 0 and 1

They get clamped if its `> 1`

### > GetBlend( )
returns blend: **number [0, 1]**

### > SetBlend( blend: **number [0, 1]** )
The **blend** parameter must be between 0 and 1

Anything else and it gets clamped to the valid range

### > ForcedMaterialOverride( mat: **Material?** )

### > GetMaterialOverride( )
returns **Material**

### > DrawScreenSpaceRectangle( destX: **int**, destY: **int**, width: **int**, height: **int**, srcTextureX1: **number**, srcTextureY1: **number**, srcTextureX2: **number**, srcTextureY2: **number**, srcTextureWidth: **int**, srcTextureHeight: **int** )

### > DrawScreenSpaceQuad( mat: **Material** )

### > GetViewport( )
returns x: **int**, y: **int**, width: **int**, height: **int**

### > DepthRange( zNear: **number**, zFar: **number** )

### > GetDepthRange( )
returns zNear: **number**, zFar: **number**

### > SetRenderTarget( tex: **Texture** )

### > GetRenderTarget( )
returns **Texture**

### > ClearBuffers( bClearColor: **bool**, bClearDepth: **bool**, bClearStencil: **bool** )

### > ClearColor3ub( red: **int**, green: **int**, blue: **int** )

### > ClearColor4ub( red: **int**, green: **int**, blue: **int**, alpha: **int** )

### > OverrideDepthEnable( enabled: **bool**, depthEnabled: **bool** )

### > OverrideAlphaEnable( enabled: **bool**, alphaEnabled: **bool**  )

### > OverrideColorEnable( enabled: **bool**, colorEnabled: **bool**  )

### > PushRenderTargetAndViewport( )
Pushes current Render Target and Viewport to the stack

### > PopRenderTargetAndViewport( )
Pops current Render Target and Viewport from the stack

### > SetStencilEnable( enabled: **bool** )

### > SetStencilFailOperation( op: **StencilOperation** )

### > SetStencilZFailOperation( op: **StencilOperation** )

### > SetStencilPassOperation( op: **StencilOperation** )

### > SetStencilCompareFunction( func: **StencilCompareFunction** )

### > SetStencilReferenceValue( value: **int** )

### > SetStencilTestMask( mask: **int** )

### > SetStencilWriteMask( mask: **int**  )

### > ClearStencilBufferRectangle( xmin: **int**, ymin: **int**, xmax: **int**, ymax: **int** )

### > Viewport( x: **int**, y: **int**, width: **int**, height: **int** )
Sets the **x**, **y**, **width** and **height** of the current viewport
