//
//  main.cpp
//  SSDSpeedTest
//
//  Created by SeanLi on 12/31/24.
//

#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <filesystem>

using namespace std;

const string TEST_FILE = "/Volumes/PSSD/test_speed.bin";
const size_t TEST_SIZE_MB = 512;
const size_t BLOCK_SIZE = 4 * 1024 * 1024;

void write_test() {
    vector<char> buffer(BLOCK_SIZE, '0');
    ofstream file(TEST_FILE, ios::binary);

    if (!file) {
        cerr << "Err file not able to be created " << TEST_FILE << endl;
        exit(1);
    }

    size_t total_written = 0;
    auto start = chrono::high_resolution_clock::now();
    while (total_written < TEST_SIZE_MB * 1024 * 1024) {
        file.write(buffer.data(), BLOCK_SIZE);
        total_written += BLOCK_SIZE;
    }
    file.close();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    double bandwidth = (TEST_SIZE_MB / elapsed.count());
    cout << "I: " << bandwidth << " MB/s" << endl;
}

void read_test() {
    vector<char> buffer(BLOCK_SIZE);
    ifstream file(TEST_FILE, ios::binary);

    if (!file) {
        cerr << "Error: Unable to open test file at " << TEST_FILE << endl;
        exit(1);
    }

    size_t total_read = 0;
    auto start = chrono::high_resolution_clock::now();
    while (file.read(buffer.data(), BLOCK_SIZE)) {
        total_read += BLOCK_SIZE;
    }
    file.close();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    double bandwidth = (TEST_SIZE_MB / elapsed.count());
    cout << "O: " << bandwidth << " MB/s" << endl;
}

int main() {
    if (!filesystem::exists("/Volumes/PSSD")) {
        cerr << "Error: Mountpoint /Volumes/PSSD not found." << endl;
        return 1;
    }

    cout << "Testing I speed..." << endl;
    write_test();

    cout << "Testing O speed..." << endl;
    read_test();

    // Clean up test file
    filesystem::remove(TEST_FILE);
    cout << "Test completed. Temporary file deleted." << endl;

    return 0;
}
