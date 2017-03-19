import cs50

# take in float from user, requiring it be positive
while True:
    print("Please enter the amount of change due in dollars:", end=" ")
    change = cs50.get_float()
    if change > 0:
        break

# convert to cents and then into an int
change = change*100
int(change)
coins = 0

# if a quarter can be given then subtract 25 and increase coins count by 1 continuing until no more quarters can be returned
while change > 24:
    change -= 25
    coins += 1

# if a dime can be given then subtract 10 and increase coins count by 1 continuing until no more dimes can be returned
while change > 9:
    change -= 10
    coins += 1

# if a nickel can be given then subtract 5 and crease coins count by 1, continuing until no more nickels can be returned
while change > 4:
    change -= 5
    coins += 1

# if a penny can be given then subtract 1 and crease coins count by 1, continuing until no more pennies can be returned
while change > 0:
    change -= 1
    coins += 1

# print total number of coins
print(coins)