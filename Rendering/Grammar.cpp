#include <unordered_map>
#include <string.h>
#include <vector>
#include <random>

struct grammar {
  std::unordered_map<std::string, std::vector<std::pair<float, std::string> > > mapping;

  void insert(std::string x, std::string y) {
    mapping[x].push_back(std::make_pair(1.0f, y));
  }

  void insert(std::string x, std::string y, float p) {
    mapping[x].push_back(std::make_pair(p, y));
  }

  std::string get_next(std::string x) {
    float p = 0.0;
    float rando = ((float) rand() / (RAND_MAX));
    // std::cout << rando << std::endl;
    for (auto e : mapping[x]) {
      p += e.first;
      if (p >= rando) {
        return e.second;
      }
    }
    return mapping[x][0].second;
  }

  std::string gen(int n, std::string inp) {
    if (n==0) {
      return inp;
    } else {
      std::string temp="";
      for (auto ch:inp) {
        std::string t;
        t.push_back(ch);
        temp += get_next(t);
      }
      return gen(n-1, temp);
    }
  }

};