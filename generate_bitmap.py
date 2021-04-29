from PIL import Image

def main():
    with open("left_map3.out", "rb") as raw_left, open("right_map3.out", "rb") as raw_right:
        arr_left = bytearray(raw_left.read())
        arr_right = bytearray(raw_right.read())

        img = Image.new('1', (240, 120), "white")
        pixels = img.load()

        for y in range(120):
            for x in range(120):
                pixels[x, y] = arr_left[x + y * 120]
                pixels[x + 120, y] = arr_right[x + y * 120]

        img.save("test.bmp")

if __name__ == "__main__":
    main()