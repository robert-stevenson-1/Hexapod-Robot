import math


def angleToCoordinate(a, b, c, l1=33, l2=90, l3=110):
    a = math.pi / 180 * a
    b = math.pi / 180 * b
    c = math.pi / 180 * c
    ox = round(l3 * math.sin(b + c) + l2 * math.sin(b))
    oy = round(l3 * math.sin(a) * math.cos(b + c) + l2 * math.sin(a) * math.cos(b) + l1 * math.sin(a))
    oz = round(l3 * math.cos(a) * math.cos(b + c) + l2 * math.cos(a) * math.cos(b) + l1 * math.cos(a))
    return ox, oy, oz


def coordinateToAngle(ox, oy, oz, l1=33, l2=90, l3=110):
    a = math.pi / 2 - math.atan2(oz, oy)
    x_3 = 0
    x_4 = l1 * math.sin(a)
    x_5 = l1 * math.cos(a)
    l23 = math.sqrt((oz - x_5) ** 2 + (oy - x_4) ** 2 + (ox - x_3) ** 2)
    w = (ox - x_3) / l23
    v = (l2 * l2 + l23 * l23 - l3 * l3) / (2 * l2 * l23)
    u = (l2 ** 2 + l3 ** 2 - l23 ** 2) / (2 * l3 * l2)
    b = math.asin(round(w, 2)) - math.acos(round(v, 2))
    c = math.pi - math.acos(round(u, 2))
    a = round(math.degrees(a))
    b = round(math.degrees(b))
    c = round(math.degrees(c))
    return a, b, c


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    x, y, z = angleToCoordinate(90, 90, 90)
    print(x)
    print(y)
    print(z)

    h, l, n = coordinateToAngle(0, 0, 0)
    print(h)
    print(l)
    print(n)
