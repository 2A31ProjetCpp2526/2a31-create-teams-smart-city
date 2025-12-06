#include "qrcodegen.hpp"

// This is a very small placeholder implementation that does NOT create a real QR code.
// It simply creates a dummy 21x21 pattern so that the UI can display a matrix-like image.
// For a production project, replace this with the full Nayuki QR-Code-generator implementation.

namespace qrcodegen {

static std::vector<bool> makeDummyPattern(int size) {
    std::vector<bool> v(size * size, false);
    // Simple frame
    for (int x = 0; x < size; ++x) {
        v[x] = true;
        v[(size - 1) * size + x] = true;
    }
    for (int y = 0; y < size; ++y) {
        v[y * size] = true;
        v[y * size + (size - 1)] = true;
    }
    // Diagonal pattern
    for (int i = 0; i < size; ++i)
        v[i * size + i] = true;
    return v;
}

QrCode QrCode::encodeText(const char * /*text*/, Ecc /*ecl*/) {
    const int sz = 21; // version 1 size
    return QrCode(sz, makeDummyPattern(sz));
}

} // namespace qrcodegen
