#include "Engine/Utils/FileUtils/IBinaryWriter.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//ENDIANNESS
//---------------------------------------------------------------------------------------------------------------------------
void IBinaryWriter::SetEndianness(eEndianness newEnd) {
	m_endianness = newEnd;
}

//---------------------------------------------------------------------------------------------------------------------------
void IBinaryWriter::ByteSwap(void* data, size_t dataSize) {
	byte* byteData = static_cast<byte*>(data);

	byte tmp;
	for (uint i = 0; i < dataSize / 2; i++) {
		tmp = byteData[dataSize - i - 1];
		byteData[dataSize - i - 1] = byteData[i];
		byteData[i] = tmp;
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//WRITE FUNCTIONS
//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool Write(const T& v) {
	T copy = v;
	if (GetLocalEndianness() != mode) {
		ByteSwap(&copy, sizeof(T));
	}
	return WriteBytes(&v, sizeof(T)) == sizeof(T);
}


bool IBinaryWriter::WriteString(const String& str) {
	for (unsigned int i = 0; i < str.length(); i++) {
		if (!Write<char>(str[i])) {
			return false;
		}
	}

	Write<char>('\0');

	return true;
}

bool IBinaryWriter::WriteChar(char data) {
	return Write<char>(data);
}
bool IBinaryWriter::WriteInt(int data) {
	return Write<int>(data);
}
bool IBinaryWriter::WriteUnsignedInt(uint data) {
	return Write<uint>(data);
}
bool IBinaryWriter::WriteByte(byte data) {
	return Write<byte>(data);
}
bool IBinaryWriter::WriteFloat(float data) {
	return Write<float>(data);
}
bool IBinaryWriter::WriteDouble(double data) {
	return Write<double>(data);
}

bool IBinaryWriter::WriteMatrix(const Matrix4& matrix) {
	return (Write<float>(matrix.matrix[0])
		&& Write<float>(matrix.matrix[1])
		&& Write<float>(matrix.matrix[2])
		&& Write<float>(matrix.matrix[3])
		
		&& Write<float>(matrix.matrix[4])
		&& Write<float>(matrix.matrix[5])
		&& Write<float>(matrix.matrix[6])
		&& Write<float>(matrix.matrix[7])
		
		&& Write<float>(matrix.matrix[8])
		&& Write<float>(matrix.matrix[9])
		&& Write<float>(matrix.matrix[10])
		&& Write<float>(matrix.matrix[11])
		
		&& Write<float>(matrix.matrix[12])
		&& Write<float>(matrix.matrix[13])
		&& Write<float>(matrix.matrix[14])
		&& Write<float>(matrix.matrix[15]));
}