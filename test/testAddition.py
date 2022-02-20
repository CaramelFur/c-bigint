from binascii import hexlify
import random
import subprocess

from matplotlib.pyplot import hexbin

while True:
    a_unsigned = random.randint(0, 2**250)
    b_unsigned = random.randint(0, 2**200)

    # print("a_unsigned:", hex(a_unsigned))
    # print("b_unsigned:", hex(b_unsigned))

    a_sign = random.randint(0, 1)
    b_sign = random.randint(0, 1)

    a_full = a_unsigned if a_sign == 0 else -a_unsigned
    b_full = b_unsigned if b_sign == 0 else -b_unsigned

    ab_full = a_full + b_full

    # Help for program:
    #   Usage: bigint <sign a> <value a hex> <sign b> <value b hex>
    # Call this program with the above arguments and it will print the result.
    # Strip 0x from the hex strings.
    send = ["../build/bigint", str(a_sign), hex(a_unsigned)
            [2:], str(b_sign), hex(b_unsigned)[2:]]
    output = subprocess.check_output(
        send
    )
    output = output.decode("utf-8").split("\n")[0]

    if(output == "neg"):
        print("Success")
        continue

    parsed = int(output, 16)


    if(parsed == ab_full):
        #print("Test passed")
        continue
    else:
        print("Test failed")
        print("Sent:     " + send.join(" "))
        print("Expected: " + hex(ab_full))
        print("Got:      " + hex(parsed))
        print("a:        " + hex(a_full))
        print("b:        " + hex(b_full))
        exit(1);
