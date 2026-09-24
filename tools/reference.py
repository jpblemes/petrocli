"""Prints the mean api_gravity from data/data.csv."""

import csv
from pathlib import Path

DATA_PATH = Path(__file__).resolve().parent.parent / "data" / "data.csv"


def main() -> None:
    with DATA_PATH.open(newline="", encoding="ascii") as csv_file:
        values = [float(row["api_gravity"]) for row in csv.DictReader(csv_file)]

    print(f"mean_api_gravity={sum(values) / len(values)}")


if __name__ == "__main__":
    main()
