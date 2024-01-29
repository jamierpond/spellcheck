#include "words.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <future>

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
  auto filteredDict = std::vector<std::string_view>{};
  filteredDict.reserve(dictionary.size() / 100);
  const auto wordLength = word.size();
  const auto oneLess = std::max(wordLength - 1, size_t{0});
  const auto oneMore = wordLength + 1;
  for (auto const &dictWord : dictionary) {
    const auto dictWordLength = dictWord.size();
    if (dictWordLength < oneLess || dictWordLength > oneMore) {
      continue;
    }
    filteredDict.push_back(dictWord);
  }

  auto closeWords = std::vector<std::string_view>{};
  closeWords.reserve(20);
  for (auto const &dictWord : filteredDict) {
    if (wagnerFischer(word, dictWord) <= 1) {
      closeWords.push_back(dictWord);
    }
  }
  return closeWords;
}

// static_assert(getCloseWords(std::string_view{"kitten"}, DICTIONARY).size() >
// 0);

int main() {
  const auto firstHalfOfDictionary = DICTIONARY.size() / 2;
  const auto secondHalfOfDictionary = DICTIONARY.size() - firstHalfOfDictionary;

  std::array<std::string_view, firstHalfOfDictionary> firstHalf;
  std::array<std::string_view, secondHalfOfDictionary> secondHalf;

  std::copy(DICTIONARY.begin(), DICTIONARY.begin() + firstHalfOfDictionary,
            firstHalf.begin());

  std::copy(DICTIONARY.begin() + firstHalfOfDictionary, DICTIONARY.end(),
            secondHalf.begin());

  // launch first thread
  auto first = std::async(std::launch::async, [&firstHalf]() {
    for (auto& word : firstHalf) {
      const auto closeWords = getCloseWords(word, DICTIONARY);
      std::cout << "Num close words for " << word << ": " << closeWords.size()
                << '\n';
    }
  });

  auto second = std::async(std::launch::async, [&secondHalf]() {
    for (auto& word : secondHalf) {
      const auto closeWords = getCloseWords(word, DICTIONARY);
      std::cout << "Num close words for " << word << ": " << closeWords.size()
                << '\n';
    }
  });


  first.wait();
  second.wait();
}
