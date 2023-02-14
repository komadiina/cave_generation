import cv2 as cv
import argparse


def main():
    parser = argparse.ArgumentParser(
        prog="thresholder",
        description="a cavegen add-in, used for thresholding a given cave using OpenCV2"
    )

    # initialize commandline arguments: filename, lower-thresh, upper-thresh
    parser.add_argument('--filename', type=str, required=True,
                        help='input image (absolute) path')
    parser.add_argument('--lower', type=int, required=True,
                        help='lower threshold value (0-255)')
    parser.add_argument('--upper', type=int, required=True,
                        help='upper threshold value (0-255)')

    # initialize parsing object
    args = parser.parse_args()

    # load image provided through commandline
    img = cv.imread(args.filename, 0)

    # threshold the image
    adaptive = cv.adaptiveThreshold(
        img, args.upper, cv.ADAPTIVE_THRESH_MEAN_C, cv.THRESH_BINARY, 11, 2)
    _, binary = cv.threshold(img, args.lower, args.upper, cv.THRESH_BINARY)

    # write the resulting image
    cv.imwrite('adaptive_' + args.filename, adaptive)
    cv.imwrite('binary_' + args.filename, binary)

    return 0


if __name__ == '__main__':
    main()
