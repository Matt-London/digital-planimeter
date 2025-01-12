import random


def greens_theorem_area(points: list[tuple[int, int]]) -> int:
    """
    Calculate the area across the shape described by `points` using Green's Theorem

    A = \\frac{1}{2}\\Sigma(x\\Delta y-y\\Delta x)

    :param points: List of points with pixel locations
    :return: Area of shape in pixels^2
    """
    if len(points) == 0:
        print("Cannot calculate area of shape with 0 points")
        return -1
    
    total_area = 0

    last_point = (0, 0)
    for point in points + [points[0]]:
        x = int(point[0] / 10)
        y = int(point[1] / 10)

        dx = x - int(last_point[0] / 10)
        dy = y - int(last_point[1] / 10)

        total_area += x * dy - y * dx
        last_point = point

    # Cough fixed clockwise vs counter-clockwise
    return abs(total_area / 2)

def is_point_in_shape(point: tuple[int, int], shape: list[tuple[int, int]]) -> bool:
    """
    Determines if a given point is inside of the shape

    :param point: Point to test
    :param shape: List of points with pixel locations
    :return: If point is located in the shape
    """
    # TODO implement this function (the hard part lol)
    return False

def monte_carlo_area(points: list[tuple[int, int]], estimators: int = 10000) -> int:
    """
    A monte carlo area estimator which generates random points in a bounded rectangle and uses
    the ratio to calculate the area

    :param points: List of points with pixel locations
    :param estimators: Number of random points to generate
    :return: Area of shape in pixels^2
    """
    if len(points) == 0:
        print("Cannot calculate area of shape with 0 points")
        return -1
    
    # Loop through and grab max left, max right, max top, max bottom
    # These will create the bounded rectangle
    max_left = points[0][0]
    max_right = points[0][0]
    max_top = points[0][1]
    max_bottom = points[0][1]

    known_area = (max_right - max_left) * (max_bottom - max_top)

    for point in points:
        max_left = min(max_left, point[0])
        max_right = max(max_right, point[0])
        max_top = min(max_top, point[1])
        max_bottom = min(max_bottom, point[1])

    # Now we have the bounding rectangle, generate points
    points_inside = 0
    for _ in range(estimators):
        rand_x = random.uniform(max_left, max_right)
        rand_y = random.uniform(max_top, max_bottom)

        if is_point_in_shape((rand_x, rand_y), points):
            points_inside += 1
    
    return known_area * (points_inside / estimators)
