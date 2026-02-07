def find_smallest(arr):
    if not arr:
        raise ValueError("Cannot find smallest element in an empty list")
    
    smallest = arr[0]
    for element in arr[1:]:
        if element < smallest:
            smallest = element
    
    return smallest


def bubble_sort(arr):
    n = len(arr)
    
    for i in range(n):
        swapped = False
        
        for j in range(0, n - i - 1):
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]
                swapped = True
        
        if not swapped:
            break
    
    return arr


def multiply_matrices(matrix1, matrix2):
    if not matrix1 or not matrix2:
        raise ValueError("Matrices cannot be empty")
    
    rows1 = len(matrix1)
    cols1 = len(matrix1[0])
    rows2 = len(matrix2)
    cols2 = len(matrix2[0])
    
    if cols1 != rows2:
        raise ValueError(f"Cannot multiply matrices: columns of first matrix ({cols1}) "
                        f"must equal rows of second matrix ({rows2})")
    
    result = [[0 for _ in range(cols2)] for _ in range(rows1)]
    
    for i in range(rows1):
        for j in range(cols2):
            for k in range(cols1):
                result[i][j] += matrix1[i][k] * matrix2[k][j]
    
    return result


def print_matrix(matrix):
    for row in matrix:
        print(" ".join(f"{elem:6}" for elem in row))


def main():    
    while True:
        print("\n" + "="*50)
        print("Select an option:")
        print("1. Find smallest element in a list")
        print("2. Bubble sort a list")
        print("3. Multiply two matrices")
        print("4. Exit")
        print("="*50)
        
        try:
            choice = int(input("Enter your choice (1-4): "))
            
            if choice == 1:
                print("\n--- Find Smallest Element ---")
                n = int(input("Enter the number of elements: "))
                arr = []
                print("Enter the elements:")
                for i in range(n):
                    arr.append(float(input(f"Element {i+1}: ")))
                
                smallest = find_smallest(arr)
                print(f"\nThe smallest element is: {smallest}")
            
            elif choice == 2:
                print("\n--- Bubble Sort ---")
                n = int(input("Enter the number of elements: "))
                arr = []
                print("Enter the elements:")
                for i in range(n):
                    arr.append(float(input(f"Element {i+1}: ")))
                
                print(f"\nOriginal list: {arr}")
                sorted_arr = bubble_sort(arr.copy())
                print(f"Sorted list: {sorted_arr}")
            
            elif choice == 3:
                print("\n--- Matrix Multiplication ---")
                
                rows1 = int(input("Enter number of rows for matrix 1: "))
                cols1 = int(input("Enter number of columns for matrix 1: "))
                
                print("Enter elements of matrix 1:")
                matrix1 = []
                for i in range(rows1):
                    row = []
                    for j in range(cols1):
                        row.append(float(input(f"Element [{i+1}][{j+1}]: ")))
                    matrix1.append(row)
                
                rows2 = int(input("\nEnter number of rows for matrix 2: "))
                cols2 = int(input("Enter number of columns for matrix 2: "))
                
                print("Enter elements of matrix 2:")
                matrix2 = []
                for i in range(rows2):
                    row = []
                    for j in range(cols2):
                        row.append(float(input(f"Element [{i+1}][{j+1}]: ")))
                    matrix2.append(row)
                
                result = multiply_matrices(matrix1, matrix2)
                
                print("\nMatrix 1:")
                print_matrix(matrix1)
                print("\nMatrix 2:")
                print_matrix(matrix2)
                print("\nResult (Matrix 1 × Matrix 2):")
                print_matrix(result)
            
            elif choice == 4:
                print("\nExiting program. Goodbye!")
                break
            
            else:
                print("Invalid choice! Please enter a number between 1 and 4.")
        
        except ValueError as e:
            print(f"\nError: {e}")
        except Exception as e:
            print(f"\nAn unexpected error occurred: {e}")


if __name__ == "__main__":
    main()
