from PIL import Image

def is_black(pixel, threshold=100):
    # Check if the pixel is considered black based on the sum of its RGB values
    return sum(pixel) < threshold

def bmp_to_binary_array(file_path):
    # Open BMP file
    image = Image.open(file_path)
    pixels = list(image.getdata())

    # Calculate number of characters
    num_chars = image.width // 8

    # Generate output
    output = []
    for i in range(num_chars):
        char_label = chr(ord('a') + i) + "_char"  # Labels: a_char, b_char, c_char, ...
        output.append(f".var {char_label}")

        for x in range(i * 8, (i + 1) * 8):  # Iterate along x axis
            byte_value = 0b1  # Initialize with the highest bit set to 1

            for y in range(image.height-1, -1, -1):  # Iterate from bottom to top
                pixel = pixels[y * image.width + x][:3]  # Extract RGB values
                bit = 1 if is_black(pixel) else 0
                byte_value = (byte_value << 1) | bit

            output.append(f"    0b{byte_value:016b}")

    return output

def save_to_file(output, file_path):
    with open(file_path, 'w') as file:
        file.write('\n'.join(output))

if __name__ == "__main__":
    input_bmp = "your_input.bmp"
    output_file = "output.txt"
    
    binary_array = bmp_to_binary_array(input_bmp)
    save_to_file(binary_array, output_file)