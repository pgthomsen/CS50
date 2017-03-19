# import cs50 won't work, error importing module below
# Traceback (most recent call last):
#  File "mario.py", line 1, in <module>
#    import cs50
#  File "/usr/lib/python3/dist-packages/cs50/__init__.py", line 2, in <module>
#    from .sql import *
#  File "/usr/lib/python3/dist-packages/cs50/sql.py", line 1, in <module>
#    import sqlalchemy
# ImportError: No module named 'sqlalchemy'

# take input from the user, accepting only integers between 0 and 23
while True:
    i = input("Please input a non-negative integer that is less than 23: ")
    i = int(i)
    if i < 23 and i > 0:
        break

# print out the collection of # in a pyramid
for j in range(2,i+1):
    print(" "*(i-j), end="")
    print("#"*j)