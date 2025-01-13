#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <filesystem>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <stdexcept>

namespace fs = std::filesystem;

bool starts_with(const std::string& str, const std::string& prefix) {
    if (str.size() < prefix.size()) {
        return false;
    }
    return str.substr(0, prefix.size()) == prefix;
}

void read_cpu_frequencies(std::map<std::string, std::vector<long>>& cpu_frequencies) {
    // Directory containing CPU frequency files
    std::string cpu_dir = "/sys/devices/system/cpu/";
    
    // List all CPU directories
    for (const auto& entry : fs::directory_iterator(cpu_dir)) {
        if (entry.is_directory() && starts_with(entry.path().filename().string(), "cpu")) {
            std::string cpu_name = entry.path().filename().string();
            std::string freq_file = entry.path().string() + "/cpufreq/scaling_cur_freq";
            std::ifstream freq_stream(freq_file);
            if (freq_stream.is_open()) {
                long freq;
                freq_stream >> freq;
                cpu_frequencies[cpu_name].push_back(freq);
                freq_stream.close();
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <total_time_seconds> <interval_milliseconds> <output_file>" << std::endl;
        return 1;
    }

    // Parse command-line arguments
    int total_time_seconds = std::atoi(argv[1]);
    int interval_milliseconds = std::atoi(argv[2]);
    std::string output_file = argv[3];

    if (total_time_seconds <= 0 || interval_milliseconds <= 0) {
        std::cerr << "Total time must be a positive integer and interval must be a positive integer in milliseconds." << std::endl;
        return 1;
    }

    // Map to store CPU frequencies
    std::map<std::string, std::vector<long>> cpu_frequencies;

    // Start time
    auto start_time = std::chrono::steady_clock::now();

    std::cout << "start recording" << std::endl;
    while (true) {
        read_cpu_frequencies(cpu_frequencies);

        // Check if the total time has elapsed
        auto current_time = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count() >= total_time_seconds) {
            break;
        }

        // Sleep for the specified interval
        std::this_thread::sleep_for(std::chrono::milliseconds(interval_milliseconds));
    }

    // Open the output file
    std::ofstream outfile(output_file);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open output file: " << output_file << std::endl;
        return 1;
    }

    // Write the final state of the map to the output file
    for (const auto& [cpu, frequencies] : cpu_frequencies) {
        outfile << cpu;
        for (long freq : frequencies) {
            outfile << "," << freq;
        }
        outfile << std::endl;
    }

    outfile.close();

    return 0;
}