#include "canvas2d.h"
#include <QPainter>
#include <QMessageBox>
#include <QFileDialog>
#include <iostream>
#include <queue>
#include "settings.h"

/**
 * @brief Initializes new 500x500 canvas
 */
void Canvas2D::init() {
    setMouseTracking(true);
    m_width = 500;
    m_height = 500;
    clearCanvas();
}

/**
 * @brief Canvas2D::clearCanvas sets all canvas pixels to blank white
 */
void Canvas2D::clearCanvas() {
    m_data.assign(m_width * m_height, RGBA{255, 255, 255, 255});
    settings.imagePath = "";
    displayImage();
}

/**
 * @brief Stores the image specified from the input file in this class's
 * `std::vector<RGBA> m_image`.
 * Also saves the image width and height to canvas width and height respectively.
 * @param file: file path to an image
 * @return True if successfully loads image, False otherwise.
 */
bool Canvas2D::loadImageFromFile(const QString &file) {
    QImage myImage;
    if (!myImage.load(file)) {
        std::cout<<"Failed to load in image"<<std::endl;
        return false;
    }
    myImage = myImage.convertToFormat(QImage::Format_RGBX8888);
    m_width = myImage.width();
    m_height = myImage.height();
    QByteArray arr = QByteArray::fromRawData((const char*) myImage.bits(), myImage.sizeInBytes());

    m_data.clear();
    m_data.reserve(m_width * m_height);
    for (int i = 0; i < arr.size() / 4; i++){
        m_data.push_back(RGBA{(std::uint8_t) arr[4*i], (std::uint8_t) arr[4*i+1], (std::uint8_t) arr[4*i+2], (std::uint8_t) arr[4*i+3]});
    }
    displayImage();
    return true;
}

/**
 * @brief Saves the current canvas image to the specified file path.
 * @param file: file path to save image to
 * @return True if successfully saves image, False otherwise.
 */
bool Canvas2D::saveImageToFile(const QString &file) {
    QImage myImage = QImage(m_width, m_height, QImage::Format_RGBX8888);
    for (int i = 0; i < m_data.size(); i++){
        myImage.setPixelColor(i % m_width, i / m_width, QColor(m_data[i].r, m_data[i].g, m_data[i].b, m_data[i].a));
    }
    if (!myImage.save(file)) {
        std::cout<<"Failed to save image"<<std::endl;
        return false;
    }
    return true;
}


/**
 * @brief Get Canvas2D's image data and display this to the GUI
 */
void Canvas2D::displayImage() {
    QByteArray img(reinterpret_cast<const char *>(m_data.data()), 4 * m_data.size());
    QImage now = QImage((const uchar*)img.data(), m_width, m_height, QImage::Format_RGBX8888);
    setPixmap(QPixmap::fromImage(now));
    setFixedSize(m_width, m_height);
    update();
}

/**
 * @brief Canvas2D::resize resizes canvas to new width and height
 * @param w
 * @param h
 */
void Canvas2D::resize(int w, int h) {
    m_width = w;
    m_height = h;
    m_data.resize(w * h);
    displayImage();
}

/**
 * @brief Called when the filter button is pressed in the UI
 */
void Canvas2D::filterImage() {
    // Filter TODO: apply the currently selected filter to the loaded image
}

/**
 * @brief Called when any of the parameters in the UI are modified.
 */
void Canvas2D::settingsChanged() {
    // this saves your UI settings locally to load next time you run the program
    settings.saveSettings();

    // TODO: fill in what you need to do when brush or filter parameters change
    m_brushRadius = settings.brushRadius; // getting updated brush radius
}

int Canvas2D::posToIndex(int x, int y) {
    // Using row-major order to calculate index
    int index = x + (y * m_width);
    return index;
}

RGBA Canvas2D::color(const RGBA &original, const RGBA &brush, float maskValue) {
    float alphaMix = maskValue * (brush.a / 255.0f);

    RGBA color;
    color.r = static_cast<std::uint8_t>(original.r * (1.0f - alphaMix) + brush.r * alphaMix + 0.5f);
    color.g = static_cast<std::uint8_t>(original.g * (1.0f - alphaMix) + brush.g * alphaMix + 0.5f);
    color.b = static_cast<std::uint8_t>(original.b * (1.0f - alphaMix) + brush.b * alphaMix + 0.5f);
    color.a = original.a;

    return color;
}

float Canvas2D::getMaskValue(float distance, int radius) {
    float A = 1.0f / (radius * radius);
    float B = -2.0f / radius;
    float C = 1.0f;

    switch (settings.brushType) {
    case BRUSH_CONSTANT:
        return 1.0f;
        break;
    case BRUSH_LINEAR:
    case BRUSH_SMUDGE:
        return 1.0f - (distance / radius);
        break;
    case BRUSH_QUADRATIC:
        return (A * distance * distance) + (B * distance) + C;
        break;
    default:
        return 1.0f;
        break;
    }
}

void Canvas2D::applyBrush(int x, int y) {
    int R = m_brushRadius;

    for (int i = -R; i <= R; i++) {
        for (int j = -R; j <= R; j++) {
            int brushX = x + i; // current pixel's x value within brush mask
            int brushY = y + j; // current pixel's y value within brush mask

            // checking if brush x and y values are within canvas bounds
            if (brushX >= 0 && brushX < m_width && brushY >= 0 && brushY < m_height) {
                float distance = sqrt(i * i + j * j);

                // checking if pixel is within radius length of brush center
                if (distance <= R) {
                    int index = posToIndex(brushX, brushY);
                    float maskValue = getMaskValue(distance, R); // mask value based on brush type

                    RGBA colorToApply;
                    if (settings.brushType == BRUSH_SMUDGE) {
                        colorToApply = m_tempColor[(i + R) * (2 * R + 1) + (j + R)];
                    } else {
                        colorToApply = settings.brushColor;
                    }

                    m_data[index] = color(m_data[index], colorToApply, maskValue);
                }
            }
        }
    }
}

void Canvas2D::pickUpSmudge(int x, int y) {
    int R = settings.brushRadius;
    int maskSize = (2 * R + 1) * (2 * R + 1);

    m_tempColor.clear();
    m_tempColor.resize(maskSize, RGBA{0, 0, 0, 0});

    for (int i = -R; i <= R; i++) {
        for (int j = -R; j <= R; j++) {
            int brushX = x + i;
            int brushY = y + j;

            if (brushX >= 0 && brushX < m_width && brushY >= 0 && brushY < m_height) {
                int index = posToIndex(brushX, brushY);
                m_tempColor[(i + R) * (2 * R + 1) + (j + R)] = m_data[index];
            } else {
                // when picking up on edge, fill pixels with RGBA(0,0,0,0)
                m_tempColor[(i + R) * (2 * R + 1) + (j + R)] = RGBA{0, 0, 0, 0};
            }
        }
    }
}

void Canvas2D::sprayBrush(int x, int y) {
    int R = settings.brushRadius;
    int maskDim = 2 * R + 1;
    int sprayDensity = (settings.brushDensity / 100.0) * (M_PI * R * R); // brush density percentage x area of brush

    for (int i = 0; i < sprayDensity; i++) {
        int randX = x - R + arc4random() % maskDim;
        int randY = y - R + arc4random() % maskDim;

        if (randX >= 0 && randX < m_width && randY >= 0 && randY < m_height) {
            float distance = sqrt((randX - x) * (randX - x) + (randY - y) * (randY - y));
            if (distance <= R) {
                int index = posToIndex(randX, randY);
                if (index >= 0 && index < m_data.size()) {
                    m_data[index] = settings.brushColor;
                }
            }
        }
    }
}

void Canvas2D::fillBucket(int x, int y) {
    // get color of canvas at position of the clicked pixel
    RGBA targetColor = m_data[posToIndex(x, y)];
    RGBA fillColor = settings.brushColor;

    // if current color is the fill color, fill nothing
    if (targetColor.r == fillColor.r && targetColor.g == fillColor.g && targetColor.b == fillColor.b && targetColor.a == fillColor.a) {
        return;
    }

    // queue to store pixels at coordinate
    std::queue<std::pair<int, int>> pixelQueue;
    pixelQueue.push({x, y});

    while (!pixelQueue.empty()) {
        std::pair<int, int> currentPixel = pixelQueue.front();
        int currentX = currentPixel.first;
        int currentY = currentPixel.second;
        pixelQueue.pop(); // remove current pixel from queue

        // check if the pixel is out of bounds
        if (currentX < 0 || currentX >= m_width || currentY < 0 || currentY >= m_height) {
            continue;
        }

        int index = posToIndex(currentX, currentY);

        // check if the pixel is still the target color
        if (m_data[index].r == targetColor.r && m_data[index].g == targetColor.g && m_data[index].b == targetColor.b && m_data[index].a == targetColor.a) {

            // set pixel to the fill color
            m_data[index] = fillColor;

            // add neighboring pixels to the queue
            pixelQueue.push({currentX + 1, currentY}); // right
            pixelQueue.push({currentX - 1, currentY}); // left
            pixelQueue.push({currentX, currentY + 1}); // down
            pixelQueue.push({currentX, currentY - 1}); // up
        }
    }
}

/**
 * @brief These functions are called when the mouse is clicked and dragged on the canvas
 */
void Canvas2D::mouseDown(int x, int y) {
    m_isDown = true;
    switch (settings.brushType) {
    case BRUSH_CONSTANT:
    case BRUSH_LINEAR:
    case BRUSH_QUADRATIC:
        applyBrush(x,y);
        break;
    case BRUSH_SMUDGE:
        pickUpSmudge(x,y);
        break;
    case BRUSH_SPRAY:
        sprayBrush(x, y);
        break;
    case BRUSH_FILL:
        fillBucket(x, y);
        break;
    default:
        break;
    }

    displayImage();
}

void Canvas2D::mouseDragged(int x, int y) {
    if (m_isDown == true) {
        switch (settings.brushType) {
        case BRUSH_CONSTANT:
        case BRUSH_LINEAR:
        case BRUSH_QUADRATIC:
            applyBrush(x, y);
            break;
        case BRUSH_SMUDGE:
            applyBrush(x,y);
            pickUpSmudge(x,y);
            break;
        case BRUSH_SPRAY:
            sprayBrush(x, y);
            break;
        default:
            break;
        }
    }

    displayImage();
}

void Canvas2D::mouseUp(int x, int y) {
    m_isDown = false;
}
