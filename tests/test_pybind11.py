import unittest
import example  # Import your Pybind11 module here
import typing

# For functional test
def callback_function(value):
    print(f"(Python) Callback function called with value: {value}")
    return value * 2;

def compose_string( str_list: typing.List[str]) -> str:
    return " ".join(str_list)

class MyModuleTestCase(unittest.TestCase):
    def test_addition(self):
        result = example.add(2, 3)  # Call functions from your Pybind11 module
        self.assertEqual(result, 5)  # Perform assertions to verify expected behavior

    def test_subtraction(self):
        result = example.subtract(5, 2)
        self.assertEqual(result, 3)

    def test_callback( self ):
        tmp_interface = example.MyInterface()
        tmp_interface.setCallback( callback_function )
        self.assertEqual( tmp_interface.performOperation( 10 ), 20 )
    
    def test_callback_vecstring( self ):
        res_str = example.process_strings( compose_string )
        # print(f"res_str: {res_str}")
        self.assertEqual( res_str, "Hello World from C++" )

if __name__ == '__main__':
    unittest.main()