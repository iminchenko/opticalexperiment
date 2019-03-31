import os, json

files = []

for root, directories, filenames in os.walk('.'):
    for filename in filenames:
        if (filename.endswith('.h')) or (filename.endswith('.hpp')) or (filename.endswith('.cpp')):
            files.append({'name':filename})


with open('clang-lines-filter.json', 'w') as file:
    file.write(json.dumps(files).replace(' ', ''))
