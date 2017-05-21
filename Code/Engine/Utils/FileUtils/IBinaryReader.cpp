#include "Engine/Utils/FileUtils/IBinaryReader.hpp"

bool IBinaryReader::ReadChar(char* out) {
	return Read<char>(out);
}
bool IBinaryReader::ReadInt(int* out) {
	return Read<int>(out);
}
bool IBinaryReader::ReadUnsignedInt(uint* out) {
	return Read<unsigned int>(out);
}
bool IBinaryReader::ReadByte(byte* out) {
	return Read<byte>(out);
}
bool IBinaryReader::ReadFloat(float* out) {
	return Read<float>(out);
}
bool IBinaryReader::ReadDouble(double* out) {
	return Read<double>(out);
}
bool IBinaryReader::ReadMatrix(Matrix4* out) {
	return (Read<float>(&out->matrix[0])
		&& Read<float>(&out->matrix[1])
		&& Read<float>(&out->matrix[2])
		&& Read<float>(&out->matrix[3])

		&& Read<float>(&out->matrix[4])
		&& Read<float>(&out->matrix[5])
		&& Read<float>(&out->matrix[6])
		&& Read<float>(&out->matrix[7])

		&& Read<float>(&out->matrix[8])
		&& Read<float>(&out->matrix[9])
		&& Read<float>(&out->matrix[10])
		&& Read<float>(&out->matrix[11])

		&& Read<float>(&out->matrix[12])
		&& Read<float>(&out->matrix[13])
		&& Read<float>(&out->matrix[14])
		&& Read<float>(&out->matrix[15]));
}

bool IBinaryReader::ReadString(String* out) {
	out->clear();

	char charToAdd;
	bool finished = false;

	while (!finished) {
		Read<char>(&charToAdd);
		out->push_back(charToAdd);

		if (charToAdd == '\0') {
			finished = true;
			break;
		}
	}

	return true;
}

//---------------------------------------------------------------------------------------------------------------------------
void IBinaryReader::ByteSwap(void* data, size_t dataSize) {
	byte* byteData = static_cast<byte*>(data);

	byte tmp;
	for (uint i = 0; i < dataSize / 2; i++) {
		tmp = byteData[dataSize - i - 1];
		byteData[dataSize - i - 1] = byteData[i];
		byteData[i] = tmp;
	}
}