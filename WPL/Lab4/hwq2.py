import math


class ParenthesesValidator:    
    def __init__(self):
        self.opening = {'(', '{', '['}
        self.closing = {')', '}', ']'}
        self.pairs = {'(': ')', '{': '}', '[': ']'}
    
    def is_valid(self, s):
        stack = []
        
        for char in s:
            if char in self.opening:
                stack.append(char)
            elif char in self.closing:
                if not stack:
                    return False
                
                last_opening = stack.pop()
                if self.pairs[last_opening] != char:
                    return False
        
        return len(stack) == 0
    
    def validate_with_details(self, s):
        stack = []
        
        for i, char in enumerate(s):
            if char in self.opening:
                stack.append((char, i))
            elif char in self.closing:
                if not stack:
                    return False, f"Closing bracket '{char}' at position {i} has no matching opening bracket"
                
                last_opening, pos = stack.pop()
                if self.pairs[last_opening] != char:
                    return False, f"Mismatched brackets: '{last_opening}' at position {pos} and '{char}' at position {i}"
        
        if stack:
            unclosed = ', '.join([f"'{char}' at position {pos}" for char, pos in stack])
            return False, f"Unclosed brackets: {unclosed}"
        
        return True, "All brackets are properly matched and closed"


class StringReverser:
    def __init__(self, string=""):
        self.string = string
    
    def set_string(self, string):
        self.string = string
    
    def reverse_words(self, string=None):
        text = string if string is not None else self.string
        
        words = text.split()
        reversed_words = words[::-1]
        return ' '.join(reversed_words)
    
    def reverse_words_preserve_whitespace(self, string=None):
        text = string if string is not None else self.string
        
        words = text.split()
        return ' '.join(words[::-1])
    
    def get_string(self):
        return self.string


class Circle:    
    def __init__(self, radius):
        if radius < 0:
            raise ValueError("Radius cannot be negative")
        self.radius = radius
    
    def area(self):
        return math.pi * self.radius ** 2
    
    def perimeter(self):
        return 2 * math.pi * self.radius
    
    def set_radius(self, radius):
        if radius < 0:
            raise ValueError("Radius cannot be negative")
        self.radius = radius
    
    def get_radius(self):
        return self.radius
    
    def __str__(self):
        return f"Circle(radius={self.radius:.2f})"


def main():    
    while True:
        print("\n" + "="*60)
        print("Select an option:")
        print("1. Validate Parentheses/Brackets")
        print("2. Reverse String Word by Word")
        print("3. Circle Area and Perimeter")
        print("4. Exit")
        print("="*60)
        
        try:
            choice = int(input("Enter your choice (1-4): "))
            
            if choice == 1:
                print("\n--- Parentheses/Brackets Validator ---")
                validator = ParenthesesValidator()
                
                string = input("Enter a string with brackets (e.g., '()[]{}' or '({[)]}'): ")
                
                is_valid, message = validator.validate_with_details(string)
                
                print(f"\nInput: '{string}'")
                print(f"Valid: {is_valid}")
                print(f"Details: {message}")
            
            elif choice == 2:
                print("\n--- String Reverser (Word by Word) ---")
                reverser = StringReverser()
                
                string = input("Enter a string to reverse word by word: ")
                reverser.set_string(string)
                
                reversed_string = reverser.reverse_words()
                
                print(f"\nOriginal: '{string}'")
                print(f"Reversed: '{reversed_string}'")
            
            elif choice == 3:
                print("\n--- Circle Area and Perimeter ---")
                
                radius = float(input("Enter the radius of the circle: "))
                circle = Circle(radius)
                
                area = circle.area()
                perimeter = circle.perimeter()
                
                print(f"\n{circle}")
                print(f"Radius: {circle.get_radius():.2f} units")
                print(f"Area: {area:.4f} square units")
                print(f"Perimeter (Circumference): {perimeter:.4f} units")
            
            elif choice == 4:
                print("\nExiting program. Goodbye!")
                break
            
            else:
                print("Invalid choice! Please enter a number between 1 and 4.")
        
        except ValueError as e:
            print(f"\nError: {e}")
            print("Please enter valid input.")
        except Exception as e:
            print(f"\nAn unexpected error occurred: {e}")


if __name__ == "__main__":
    main()
