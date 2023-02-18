#include "./cave.h"

Cave::Cave(const int64_t& w, const int64_t& h)
    : _width(w)
    , _height(h)
{
    // initialize a zeroed floatmap
    _matrix = new float*[h];
    for (int i = 0; i < h; i++)
        _matrix[i] = new float[w]();
}

Cave& Cave::operator=(const Cave& rhs)
{
    // potential todo: use overwriting instead of full-on deletion and reinitialization

    for (int i = 0; i < _height; ++i)
        delete _matrix[i];
    delete _matrix;

    int64_t w = rhs.width(), h = rhs.height();
    _matrix = new float*[h];

    for (int i = 0; i < h; i++)
        _matrix[i] = new float[w]();

    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
            _matrix[y][x] = rhs.at(x, y);

    return *this;
}

// 2D Perlin noise implementation in interval [lower, upper]
void Cave::noise(float lower, float upper)
{
    auto scale = [](const float value,
                     const float oldLower,
                     const float oldUpper,
                     const float lower,
                     const float upper) {
        return std::min(std::max((value - oldLower) * (upper - lower) / (oldUpper - oldLower) + lower, lower),
            upper);
    };

    srand(time(NULL));

    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x)
            _matrix[y][x] = scale(perlin(x + rand() % 2, y + rand() % 2), 0, 1, lower, upper);
    }
}

float Cave::interpolate(float a0, float a1, float w)
{
    if (0.0 > w)
        return a0;
    if (1.0 < w)
        return a1;

    return (a1 - a0) * w + a0;
}

vector2 Cave::random_gradient(int ix, int iy)
{
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2; // rotation width
    unsigned a = ix, b = iy;

    a *= 3284157443;
    b ^= a << s | a >> w - s;
    b *= 1911520717;
    a ^= b << s | b >> w - s;
    a *= 2048419325;
    float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]

    vector2 v;
    v.x = std::cos(random);
    v.y = std::sin(random);

    return v;
}

float Cave::dotgrid_gradient(int ix, int iy, float x, float y)
{
    vector2 grad = random_gradient(ix, iy);

    float dx = x - (float)ix;
    float dy = y - (float)iy;

    return (dx * grad.x + dy * grad.y);
}

float Cave::perlin(float x, float y)
{
    int x0 = (int)std::floor(x), y0 = (int)std::floor(y);
    int x1 = x0 + 1, y1 = y0 + 1;

    float sx = x - (float)x0, sy = y - (float)y0;
    float n0, n1, ix0, ix1, value;

    n0 = dotgrid_gradient(x0, y0, x, y);
    n1 = dotgrid_gradient(x1, y0, x, y);
    ix0 = interpolate(n0, n1, sx);

    n0 = dotgrid_gradient(x0, y1, x, y);
    n1 = dotgrid_gradient(x1, y1, x, y);
    ix1 = interpolate(n0, n1, sx);

    value = interpolate(n0, n1, sx);

    return value * 0.5 + 0.5;
}

void Cave::box_blur(int radius)
{
    int size = 1 + radius * 2;
    float kernelValue = 1.0f / (size * size);

    std::vector<float> kernel(size * size, kernelValue);
    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            float value = 0.0f;

            for (int ky = -radius; ky <= radius; ++ky) {
                for (int kx = -radius; kx <= radius; ++kx) {
                    int xPos = x + kx, yPos = y + ky;

                    if (xPos < 0 || xPos >= _width || yPos < 0 || yPos >= _height)
                        continue;

                    value += _matrix[yPos][xPos] * kernel[(ky + radius) * size + kx * radius];
                }
            }

            _matrix[y][x] = value;
        }
    }
}

void Cave::carve(float minValue = 0.5)
{
    bool** visited = new bool*[_height];
    for (int i = 0; i < _width; ++i)
        visited[i] = new bool[_width]();

    for (int y = 0; y < _height; ++y)
        for (int x = 0; x < _width; ++x)
            visited[y][x] = false;

    auto is_valid = [&, this](int row, int col) {
        if (row < 0 || row >= _width || col < 0 || col >= _height)
            return false;

        return !visited[row][col];
    };

    int dRow[] = { 0, 1, 0, -1 };
    int dCol[] = { -1, 0, 1, 0 };
    std::stack<std::pair<int, int>> st;
    int startX = _width / 2, startY = _height / 2;

    // srand(time(NULL));
    // startX = rand() % _width;
    // startY = rand() % _height;
    // st.push({startX, startY});

    st.push({ startX, startY });

    printf("Carving from (x, y) = (%d, %d)...\n", startX, startY);

    while (!st.empty()) {
        auto curr = st.top();
        st.pop();

        auto row = curr.first, col = curr.second;

        if (is_valid(row, col) == false)
            continue;
        if (minValue > _matrix[row][col])
            continue;

        visited[row][col] = true;

        for (int i = 0; i < 4; i++)
            st.push({ row + dRow[i], col + dCol[i] });
    }

    for (int y = 0; y < _height; ++y)
        for (int x = 0; x < _width; ++x)
            if (visited[y][x] == false)
                _matrix[y][x] = 0.01f;
            else
                _matrix[y][x] = 0.99f;
}

float Cave::at(const int64_t& x, const int64_t& y)
{
    if (x < 0 || y < 0 || x >= _width || y >= _height)
        throw std::out_of_range("Invalid indexing attempt");

    return _matrix[y][x];
}

float Cave::at(const int64_t& x, const int64_t& y) const
{
    if (x < 0 || y < 0 || x >= _width || y >= _height)
        throw std::out_of_range("Invalid indexing attempt");

    return _matrix[y][x];
}

std::ostream& operator<<(std::ostream& os, const Cave& obj)
{
    for (int y = 0; y < obj.height(); y++) {
        for (int x = 0; x < obj.width(); x++)
            os << std::setw(8) << (obj.matrix())[y][x] << " ";

        os << std::endl;
    }

    return os;
}

/* ==================== */
/* ---- CaveImage ----- */
/* ==================== */

CaveImage::CaveImage(const Cave& cave)
    : _cave(cave)
{
    init_pixelmap();
}

void CaveImage::init_pixelmap()
{
    // Initialize an empty PixelMatrix (vector<vector<Pixel>)
    for (int i = 0; i < _cave.height(); ++i) {
        std::vector<Pixel> row(_cave.width(), Pixel(0, 0, 0));
        _pixelmap.push_back(row);
    }

    // Using the provided floatmap, transfer floatmap values into pixelmap
    // (255 * value) % 256
    for (int y = 0; y < _cave.height(); ++y)
        for (int x = 0; x < _cave.width(); ++x) {
            auto monochrome_intensity = (int)((_cave.matrix())[y][x] * 255) % 256;
            Pixel p(monochrome_intensity, monochrome_intensity, monochrome_intensity);
            _pixelmap[y][x] = p;
        }
}

void CaveImage::save(const std::string& filename)
{
    Bitmap bmp;
    bmp.fromPixelMatrix(_pixelmap);
    bmp.save(filename);
}

void CaveImage::load(const Cave& cave)
{
    _cave = cave;
    init_pixelmap();
}

void CaveImage::scale(const uint64_t& factor) { }