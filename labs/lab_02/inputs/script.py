content = ''
with open('real.txt', 'r') as fileobj:
    row = []
    for line in fileobj:
        row = line.split()
        if(row==['1000', '1000', '1000', '1000', '1000']):
            continue
        ans_row = [str(1000 - int(val)) for val in row]
        # content.append(ans_row)
        content = content + ' '.join(ans_row) + '\n'

print(content)
# with open('complement.txt', 'w') as fileobj:
#     for row in content:
