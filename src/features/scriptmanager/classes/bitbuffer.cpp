#include "../../../thirdparty/sol3/sol.hpp"
#include "../../../sdk/definitions/bitbuf.h"

#include <optional>
#include <vector>

/* Lua Example
-- create new bit buffer
local bf = BitBuffer.new()

-- write '12' to it
bf:write_int(12, 32)

-- rewind current bit position to 0 (start)
bf:set_bit_pos(0)

-- read 32 bits, which in this case is '12'
print(bf:read_int(32))

-- free memory
bf:delete()
*/

class bitbuffer
{
public:
	bitbuffer();

	int read_int(int maxbits);
	float read_float(int maxbits);
	bool read_string(char* out, int maxlength);

	int write_int(int value, int maxbits);
	int write_float(float value, int maxbits);
	int write_string(const char* value);

	void seek_to_bit(int bit_position);
	int get_current_bit() const { return curbit; }

	void sync();

	void free_memory();

private:
	void ensure_capacity(int bits_to_write);

	int curbit = 0;
	bf_read reader {};
	bf_write writer {};

	std::vector<unsigned char> pool;
};

bitbuffer::bitbuffer()
{
	pool.resize(256, 0);
	curbit = 0;
	sync();
}

void bitbuffer::sync()
{
	reader.StartReading(pool.data(), pool.size());
	reader.Seek(curbit);

	writer.StartWriting(pool.data(), pool.size());
	writer.SeekToBit(curbit);
}

void bitbuffer::ensure_capacity(int bits_to_write)
{
	int total_bits_available = pool.size() * 8;

	if (curbit + bits_to_write > total_bits_available)
	{
		size_t new_size = pool.size() * 2;

		while (new_size * 8 < (size_t)(curbit + bits_to_write))
			new_size *= 2;

		pool.resize(new_size, 0);

		sync();
	}
}

void bitbuffer::seek_to_bit(int bit_position)
{
	curbit = (bit_position < 0) ? 0 : bit_position;

	ensure_capacity(0);

	sync();
}

int bitbuffer::read_int(int maxbits)
{
	int value = reader.ReadUBitLong(maxbits);
	curbit = reader.GetNumBitsRead();
	sync();
	return value;
}

float bitbuffer::read_float(int maxbits)
{
	uint32_t raw_bits = reader.ReadUBitLong(maxbits);
	curbit = reader.GetNumBitsRead();
	sync();

	float val;
	memcpy(&val, &raw_bits, sizeof(float));
	return val;
}

bool bitbuffer::read_string(char* out, int maxlength)
{
	if (!out || maxlength <= 0)
		return false;

	bool ret = reader.ReadString(out, maxlength);
	curbit = reader.GetNumBitsRead();
	sync();

	return ret;
}

int bitbuffer::write_int(int value, int maxbits)
{
	if (maxbits <= 0) return curbit;

	ensure_capacity(maxbits);
	writer.WriteUBitLong(value, maxbits);
	curbit = writer.GetNumBitsWritten();
	sync();

	return curbit;
}

int bitbuffer::write_float(float val, int maxbits)
{
	if (maxbits <= 0) return curbit;

	ensure_capacity(maxbits);

	uint32_t raw_bits;
	memcpy(&raw_bits, &val, sizeof(float));

	writer.WriteUBitLong(raw_bits, maxbits);
	curbit = writer.GetNumBitsWritten();
	sync();

	return curbit;
}

int bitbuffer::write_string(const char* value)
{
	if (!value) return 0;

	int estimated_bits = (strlen(value) + 1) * 8;
	ensure_capacity(estimated_bits);

	writer.WriteString(value);
	curbit = writer.GetNumBitsWritten();
	sync();

	return curbit;
}

void bitbuffer::free_memory()
{
	pool.clear();
	pool.shrink_to_fit();
	curbit = 0;
	sync();
}

void BindBitBuffer(sol::state& lua)
{
	lua.new_usertype<bitbuffer>
	(
		"BitBuffer", sol::constructors<bitbuffer()>(),

		"ReadInt", [](bitbuffer& bf, std::optional<int> maxbits) -> int
		{
			return bf.read_int(maxbits.value_or(32));
		},

		"ReadFloat", [](bitbuffer& bf, std::optional<int> maxbits) -> float
		{
			return bf.read_float(maxbits.value_or(32));
		},

		"ReadString", [](bitbuffer& bf, std::optional<int> maxlen) -> std::string
		{
			int len = std::clamp(maxlen.value_or(256), 0, 512);

			std::vector<char> str;
			str.reserve(len);

			if (!bf.read_string(str.data(), len))
				return "";

			return std::string(str.data());
		},

		"WriteInt", [](bitbuffer& bf, int value, std::optional<int> maxbits) -> int
		{
			return bf.write_int(value, maxbits.value_or(32));
		},

		"WriteFloat", [](bitbuffer& bf, float value, std::optional<int> maxbits) -> int
		{
			return bf.write_float(value, maxbits.value_or(32));
		},

		"WriteString", [](bitbuffer& bf, const char* value) -> int
		{
			return bf.write_string(value);
		},

		"GetBitPos", [](bitbuffer& bf) -> int
		{
			return bf.get_current_bit();
		},

		"SetBitPos", [](bitbuffer& bf, unsigned int bit) -> void
		{
			bf.seek_to_bit(bit);
		},

		"Free", [](bitbuffer& bf) -> void
		{
			bf.free_memory();
		}
	);
}