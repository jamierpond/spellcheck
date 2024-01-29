#include "words.hpp"
#include <iostream>
#include <vector>

constexpr auto wagnerFischer = [](auto const &a, auto const &b) {
  auto const m = a.size();
  auto const n = b.size();
  auto d = std::vector<std::vector<int>>(m + 1, std::vector<int>(n + 1));
  for (auto i = 0; i <= m; ++i) {
    d[i][0] = i;
  }
  for (auto j = 0; j <= n; ++j) {
    d[0][j] = j;
  }
  for (auto j = 1; j <= n; ++j) {
    for (auto i = 1; i <= m; ++i) {
      if (a[i - 1] == b[j - 1]) {
        d[i][j] = d[i - 1][j - 1];
      } else {
        d[i][j] =
            std::min({d[i - 1][j] + 1, d[i][j - 1] + 1, d[i - 1][j - 1] + 1});
      }
    }
  }
  return d[m][n];
};

static_assert(wagnerFischer(std::string_view{"kitten"},
                            std::string_view{"sitting"}) == 3);
static_assert(wagnerFischer(std::string_view{"jamie"},
                            std::string_view{"jmie"}) == 1);

constexpr auto getCloseWords(const auto& word, const auto& dictionary) {
  auto closeWords = std::vector<std::string_view>{};
  for (auto const& dictWord : dictionary) {
    if (wagnerFischer(word, dictWord) <= 1) {
      closeWords.push_back(dictWord);
    }
  }
  return closeWords;
}

// static_assert(getCloseWords(std::string_view{"kitten"}, DICTIONARY).size() > 0);

int main() {
  constexpr auto word = std::string_view{"banana"};
  const auto closeWords = getCloseWords(word, DICTIONARY);

  std::cout << "Close words to " << word << ":\n";

  for (auto const& closeWord : closeWords) {
    std::cout << closeWord << '\n';
  }
}
