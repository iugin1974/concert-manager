#include <string>

class VimView {
public:
    bool edit(const std::string &text);
    std::string getText();

private:
    std::string newText;
};
