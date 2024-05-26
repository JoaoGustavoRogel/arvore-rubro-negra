from tqdm import tqdm
import random

def create_vector(size: int, bounds: tuple = (1, 2000000)) -> list:
    data = []
    aux = set()

    while len(data) != size:
        number = random.randint(bounds[0], bounds[1])

        if number in aux:
            continue

        data.append(number)
        aux.add(number)

    return data

def create_file() -> None:
    data = []
    with open("data/vectors", "w") as _fp:
        for n in tqdm(range(1, 10001)):
            vector = create_vector(n)
            data.append(vector)
            str_data = "".join(str(vector).replace(", ", " ").replace("]", "").replace("[", "")) + "\n"
            _fp.write(str_data)


if __name__ == "__main__":
    create_file()
