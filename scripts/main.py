import fundamentus
import os
import pandas as pd

INPUT_FILE_NAME = 'data.csv'
OUTPUT_FILE_NAME = 'out.csv'

def get_file_path(file_name):
    return os.path.join(os.path.dirname(__file__), file_name)

def get_mnemonics_from_csv():
    with open(get_file_path(INPUT_FILE_NAME), 'r') as f:
        # Assuming the first column contains mnemonics
        return [line.strip().split(',')[0] for line in f.readlines()]

mnemonics = get_mnemonics_from_csv()

df = pd.DataFrame()
for mnemonic in mnemonics:
    response = fundamentus.get_papel(mnemonic)
    if isinstance(response, pd.DataFrame):
        df = pd.concat([df, response], ignore_index=True)
    else:
        print(f'Could not find {mnemonic} in Fundamentus')

df.to_csv(get_file_path(OUTPUT_FILE_NAME), index=False)
