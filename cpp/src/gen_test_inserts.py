import random
import string
import sys

num_cases = int(sys.argv[1]) if len(sys.argv) > 0 else 100

def random_chr():
    return random.choice(string.ascii_letters + string.digits)

def random_str(n):
    return ''.join(random_chr() for _ in range(n))

for i in range(num_cases):
    print('i')
    print(random_str(random.randint(1, 20)))
    print(random_str(random.randint(1, 20)))

print('q')
