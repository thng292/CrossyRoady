import os

def hex_to_rgb(hex_code):
    hex_code = hex_code.strip('#')
    r = int(hex_code[0:2], 16)
    g = int(hex_code[2:4], 16)
    b = int(hex_code[4:6], 16)
    return f'{r},{g},{b}'

directory = '.'  # Change this to the directory containing the .hex files

for filename in os.listdir(directory):
    if filename.endswith('.hex'):
        input_file = os.path.join(directory, filename)
        output_file = os.path.splitext(input_file)[0] + '.txt'

        with open(input_file, 'r') as file:
            lines = file.readlines()

        rgb_values = [hex_to_rgb(line.strip()) for line in lines]

        with open(output_file, 'w') as file:
            for rgb in rgb_values:
                file.write(f'{rgb}\n')
