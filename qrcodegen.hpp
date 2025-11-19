// QR Code generator library (C++)
// Source: https://github.com/nayuki/QR-Code-generator (public domain / MIT-like license)
// This is a trimmed copy suitable for local use in the Smart City project.

#ifndef QRCODEGEN_HPP
#define QRCODEGEN_HPP

#include <cstdint>
#include <vector>
#include <string>

namespace qrcodegen {

class QrCode final {
public:
    enum class Ecc { LOW = 0, MEDIUM, QUARTILE, HIGH };

    // Creates a QR Code from the given text using the specified error correction level.
    static QrCode encodeText(const char *text, Ecc ecl);

    int getSize() const { return size; }
    bool getModule(int x, int y) const { return modules.at(y * size + x); }

private:
    int size;
    std::vector<bool> modules;

    QrCode(int sz, std::vector<bool> data) : size(sz), modules(std::move(data)) {}
};

} // namespace qrcodegen

#endif // QRCODEGEN_HPP
