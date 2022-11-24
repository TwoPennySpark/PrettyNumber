#include <unordered_map>
#include <iostream>
#include <numeric>

// N - base, DN - amount of digits
template <uint8_t N, uint8_t DN>
class uintBaseN
{
public:
    uintBaseN() = default;
    uintBaseN(char const (&number)[DN+1])
    {
        for (size_t i = 0;i < DN; i++)
        {
            uint8_t d = uint8_t(number[i]);
            switch (number[i])
            {
#ifdef __GNUC__
                case '0' ... '9':
#else
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
#endif
                    data[i] = d - '0';
                    break;
#ifdef __GNUC__
                case 'a' ... 'z':
#else
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
                case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
                case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
#endif
                    data[i] = d - 'a' + 0xA;
                    break;

#ifdef __GNUC__
                case 'A' ... 'Z':
#else
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
                case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
                case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
#endif
                    data[i] = d - 'A' + 0xA;
                    break;
                default:
                    data[i] = N;
                    break;
            }
            if (data[i] >= N)
                throw std::runtime_error("Invalid number");
        }
    }

    uintBaseN& operator++()
    {
        uint8_t index = DN-1;
        while (index != std::numeric_limits<decltype(index)>::max() && (++data[index] == N))
            data[index--] = 0;
        return *this;
    }

    bool operator<= (const uintBaseN<N, DN>& right) const
    {
        for (size_t i = 0; i < DN; i++)
        {
            if (data[i] < right.data[i])
                return true;
            else if (data[i] > right.data[i])
                return false;
        }
        return true;
    }

    uint64_t accumulate() const
    {
        return std::accumulate(data.begin(), data.end(), 0u);
    }

private:
    std::array<uint8_t, DN> data{};
};

int main()
{
    uint64_t nPretty = 0;
    const uint8_t BASE = 13;
    // 1st - sum, 2nd - number of occurrences
    std::unordered_map<uint64_t, uint64_t> hSumOccurs;

    // count occurrences of every sum
    for (uintBaseN<BASE, 7>i = "0000000"; i <= uintBaseN<BASE, 7>("0CCCCCC"); ++i)
        hSumOccurs[i.accumulate()]++;

    // Cartesian product
    for (auto& [_, n]: hSumOccurs)
        nPretty += n*n;

    nPretty *= BASE;

    std::cout << "[*]ANSWER:" << nPretty << "\n";

    return 0;
}
