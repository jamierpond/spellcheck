#include "words.hpp"
#include <iostream>
#include <vector>

constexpr auto wagnerFischer = [](auto const &a, auto const &b) {
  auto const width = a.size();
  auto const height = b.size();
  auto d =
      std::vector<std::vector<int>>(width + 1, std::vector<int>(height + 1));
  for (auto i = 0; i <= width; ++i) {
    d[i][0] = i;
  }
  for (auto j = 0; j <= height; ++j) {
    d[0][j] = j;
  }
  for (auto j = 1; j <= height; ++j) {
    for (auto i = 1; i <= width; ++i) {
      if (a[i - 1] == b[j - 1]) {
        d[i][j] = d[i - 1][j - 1];
      } else {
        d[i][j] =
            std::min({d[i - 1][j] + 1, d[i][j - 1] + 1, d[i - 1][j - 1] + 1});
      }
    }
  }
  return d[width][height];
};

static_assert(wagnerFischer(std::string_view{"kitten"},
                            std::string_view{"sitting"}) == 3);
static_assert(wagnerFischer(std::string_view{"jamie"},
                            std::string_view{"jmie"}) == 1);

constexpr auto getCloseWords(const auto &word, const auto &dictionary) {
  auto closeWords = std::vector<std::string_view>{};
  closeWords.reserve(50);
  for (auto const &dictWord : dictionary) {
    const auto lengthDiff = std::abs(static_cast<int>(word.size()) -
                                     static_cast<int>(dictWord.size()));
    if (lengthDiff > 1) {
      continue;
    }
    if (wagnerFischer(word, dictWord) <= 1) {
      closeWords.push_back(dictWord);
    }
  }
  return closeWords;
}

// static_assert(getCloseWords(std::string_view{"kitten"}, DICTIONARY).size() >
// 0);

int main() {
  for (auto& word : DICTIONARY) {
    const auto closeWords = getCloseWords(word, DICTIONARY);
    std::cout << "Num close words for " << word << ": " << closeWords.size()
              << '\n';
  }
}
