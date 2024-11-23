import matplotlib.pyplot as plt

def read_series_from_file(name):
    with open(name, "r") as f:
        data = [l.split() for l in f.readlines()]
        x = [int(x) for x, y in data]
        y = [int(y) for x, y in data]
    return x, y



def build_plot(title, x_m, y_m, x_m_i, y_m_i):
    plt.figure(figsize=(15, 10))
    plt.plot(x_m, y_m, label="Quick Sort")
    plt.plot(x_m_i, y_m_i, label="Intro sort")

    plt.scatter(x_m, y_m, label="points 1", s=15, alpha=0.5)
    plt.scatter(x_m_i, y_m_i, label="points 2", s=15, alpha=0.5)

    plt.xlabel("N")
    plt.ylabel("AVG_T, nanoseconds")

    plt.legend()
    plt.grid()

    plt.title(title)
    plt.savefig(f"{title}.png")

x1, y1 = read_series_from_file("q_s_test1.txt")
x2, y2 = read_series_from_file("q_s_test2.txt")
x3, y3 = read_series_from_file("q_s_test3.txt")
x4, y4 = read_series_from_file("i_s_test1.txt")
x5, y5 = read_series_from_file("i_s_test2.txt")
x6, y6 = read_series_from_file("i_s_test3.txt")


build_plot("Random Array", x1, y1, x4, y4)
build_plot("Reversed Array", x2, y2, x5, y5)
build_plot("Near Sorted Array", x3, y3, x6, y6)