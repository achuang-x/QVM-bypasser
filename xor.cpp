#include <iostream>  
#include <fstream>  
#include <vector>  

unsigned char xor_byte(unsigned char byte, unsigned char key) {
    return byte ^ key;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <input_filename> <output_filename> <key>" << std::endl;
        return 1;
    }

    std::string input_filename = argv[1];
    std::string output_filename = argv[2];
    unsigned char key = static_cast<unsigned char>(std::atoi(argv[3]));

    std::ifstream input_file(input_filename, std::ios::binary);
    std::ofstream output_file(output_filename, std::ios::binary);

    input_file.seekg(0, std::ios::end);
    size_t file_size = input_file.tellg();
    input_file.seekg(0, std::ios::beg);

    std::vector<unsigned char> file_data(file_size);
    if (!input_file.read(reinterpret_cast<char*>(file_data.data()), file_size)) {
        std::cerr << "Failed to read the input file." << std::endl;
        return 1;
    }

    for (unsigned char& byte : file_data) {
        byte = xor_byte(byte, key);
    }

    if (!output_file.write(reinterpret_cast<char*>(file_data.data()), file_data.size())) {
        std::cerr << "Failed to write the encrypted data to the output file." << std::endl;
        return 1;
    }

    std::cout << "Encryption completed. Encrypted data saved to " << output_filename << std::endl;

    return 0;
}