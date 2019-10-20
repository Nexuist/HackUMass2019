import cv2


def resize(src):
    scale_percent = 60
    width = int(src.shape[1] * scale_percent / 100)
    height = int(src.shape[0] * scale_percent / 100)
    dsize = (width, height)
    return cv2.resize(src, dsize)


def main():
    cap = cv2.VideoCapture(1)
    while True:
        ret, frame = cap.read()
        frame = resize(frame)
        cv2.imshow("TurboChair", frame)
        if cv2.waitKey(1) == 27:
            break
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
