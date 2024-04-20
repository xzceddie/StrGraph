import unittest
import example  # Import your Pybind11 module here

class MyModuleTestCase(unittest.TestCase):
    def test_addition(self):
        result = example.add(2, 3)  # Call functions from your Pybind11 module
        self.assertEqual(result, 5)  # Perform assertions to verify expected behavior

    def test_subtraction(self):
        result = example.subtract(5, 2)
        self.assertEqual(result, 3)

if __name__ == '__main__':
    unittest.main()
