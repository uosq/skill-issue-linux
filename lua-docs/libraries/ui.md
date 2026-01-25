# UI Library

Wrapper for ImGui

This library should only be used in the **ImGui** hook

## Methods

### > Begin( name: **string** )
returns **bool**

### > Button( label: **string** )
returns **bool**

The return value is true when it was clicked

### > Checkbox( label: **string**, value: **bool** )
returns changed: **bool**, new value: **bool**

### > TextUnformatted( text: **string** )

### > SliderFloat( label: **string**, value: **number**, min: **number**, max: **number** )
returns clicked: **bool**, new value: **number**

### > End( )
This should **ALWAYS** be called after **Begin**, even if it returned false!

Don't complain to me about crashes if you forget to call **End**