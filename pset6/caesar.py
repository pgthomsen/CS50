import cs50
import sys

# If improper arguments given return 1 and error message
if len(sys.argv) != 2:
    print("Please enter only a single argument to the program")
    exit(1)

# Requrest and store plaintext
print("plaintext:", end=" ")

plaintext = cs50.get_string()

k = sys.argv[1]
k=int(k)

ln = len(plaintext)

print("cyphertext:", end=" ")

# Check if the letter is upper case of lower case and rotate by k maintaining case
for i in range(ln):
    if ord(plaintext[i]) > 64 and ord(plaintext[i]) < 91:
        c = ord(plaintext[i]) - 65
        c = (c + k)%26
        c += 65
        print(chr(c), end="")
        
    elif ord(plaintext[i]) > 96 and ord(plaintext[i]) < 123:
        c = ord(plaintext[i]) - 97
        c = (c + k)%26
        c += 97
        print(chr(c), end="")
    else:
        print(plaintext[i], end="")

print("")
# end program returning 0
exit(0)