#ifndef __ZONE__
#define __ZONE__

#include <format>
#include <vector>
#include <fstream>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <filesystem>

enum struct State
{
    Start = 'X',
    Free = '0',
    Wall = '1',
    Stop = 'Y',
};

std::istream& operator>>(std::istream &is, State &s);
std::ostream& operator<<(std::ostream &os, const State &s);

template<typename S>
class Zone
{
public:
    std::vector<std::vector<S>> data;

public:
    static Zone<S> read_from(const std::filesystem::path& filepath)
    {
        std::ifstream problem_file(filepath, std::ios::in);

        if (!problem_file.is_open())
        {
            throw std::runtime_error(format("Could not open file {}!", filepath.string()));
        }

        Zone<S> zone;
        problem_file >> zone;
        problem_file.close();
        return zone;
    }
};

template<typename S>
inline std::ifstream& operator>>(std::ifstream& is, Zone<S>& zone)
{
    // Obtain size constraints
    int N, M; is >> N; is >> M;


    // Allocate all elements at once
    zone.data = std::vector<std::vector<S>>(N, std::vector<S>(M));

    // Read each state into the zone
    for (decltype(N) i = 0; i != N; ++i)
    {
        for (decltype(M) j = 0; j != M; ++j)
        {
            is >> zone.data[i][j];
        }
    }

    return is;
}

#endif