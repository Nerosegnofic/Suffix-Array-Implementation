#include <iostream>

using std::cout, std::endl;

struct SuffixComparator {
    int* group;
    int length;
    int k;

    SuffixComparator(int* group, const int length, const int k) : group{group}, length{length}, k{k} {}


    bool operator()(const int i, const int j) const {
        if (group[i] != group[j]) {
            return group[i] < group[j];
        }

        const int ri {i + k < length ? group[i + k] : -1};
        const int rj {j + k < length ? group[j + k] : -1};
        return ri < rj;
    }
};

class SuffixArray {
    const char* text;
    int length;
    int* suffix_array;
    int* temp_suffix_array;
    int* group;
    int* temp_group;

    void sort_suffix_array(const int k) const {
        const SuffixComparator comparator(group, length, k);
        merge_sort(suffix_array, 0, length - 1, comparator);
    }

    static void merge_sort(int* array, const int left, const int right, const SuffixComparator& comparator) {
        if (left >= right) {
            return;
        }

        const int mid {left + (right - left) / 2};
        merge_sort(array, left, mid, comparator);
        merge_sort(array, mid + 1, right, comparator);

        merge(array, left, mid, right, comparator);
    }

    static void merge(int* array, const int left, const int mid, const int right, const SuffixComparator& comparator) {
        const int n1 {mid - left + 1};
        const int n2 {right - mid};

        const auto left_array {new int[n1]};
        const auto right_array {new int[n2]};

        for (int i {0}; i < n1; ++i) {
            left_array[i] = array[left + i];
        }

        for (int i {0}; i < n2; ++i) {
            right_array[i] = array[mid + 1 + i];
        }

        int i {0};
        int j {0};
        int k {left};

        while (i < n1 && j < n2) {
            if (comparator(left_array[i], right_array[j])) {
                array[k] = left_array[i];
                ++k;
                ++i;
            } else {
                array[k] = right_array[j];
                ++k;
                ++j;
            }
        }

        while (i < n1) {
            array[k] = left_array[i];
            ++k;
            ++i;
        }

        while (j < n2) {
            array[k] = right_array[j];
            ++k;
            ++j;
        }

        delete[] left_array;
        delete[] right_array;
    }

public:
    explicit SuffixArray(const char* input) : text{input}, length {0} {
        while (text[length] != '\0') {
            ++length;
        }

        suffix_array = new int[length];
        group = new int[length];
        temp_group = new int[length];
        temp_suffix_array = new int[length];

        for (int i {0}; i < length; ++i) {
            suffix_array[i] = i;
            group[i] = static_cast<unsigned char>(text[i]);
        }
    }

    ~SuffixArray() {
        delete[] suffix_array;
        delete[] group;
        delete[] temp_group;
        delete[] temp_suffix_array;
    }

    void ConstructUsingPrefixDoubling() const {
        for (int k {1}; k < length; k *= 2) {
            sort_suffix_array(k);

            temp_group[suffix_array[0]] = 0;

            for (int i {1}; i < length; ++i) {
                temp_group[suffix_array[i]] = temp_group[suffix_array[i - 1]] +
                    (SuffixComparator(group, length, k)(suffix_array[i - 1], suffix_array[i]) ? 1 : 0);
            }

            for (int i {0}; i < length; ++i) {
                group[i] = temp_group[i];
            }
        }
    }

    void Print() const {
        for (int i {0}; i < length; ++i) {
            cout << suffix_array[i] << " ";
        }
        cout << endl;
    }
};

int main()
{
    SuffixArray t("ACGACTACGATAAC$");
    t.ConstructUsingPrefixDoubling();
    t.Print(); // Prints: 14 11 12 0 6 3 9 13 1 7 4 2 8 10 5
    return 0;
}
