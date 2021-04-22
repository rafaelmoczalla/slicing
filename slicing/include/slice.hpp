#include <cstdlib>
#include <string>

class Slice {
  private:
    std::string state;

  public:
    Slice();
    void finish();
    std::string get();
};