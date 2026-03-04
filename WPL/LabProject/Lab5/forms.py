from django import forms

class TextFormatterForm(forms.Form):
    # Options for styles and colors
    STYLE_CHOICES = [
        ('bold', 'BOLD'),
        ('italic', 'ITALIC'),
        ('underline', 'UNDERLINE'),
    ]
    
    COLOR_CHOICES = [
        ('red', 'RED'),
        ('green', 'GREEN'),
        ('pink', 'PINK'),
    ]

    name = forms.CharField(
        label="Enter Name", 
        max_length=100,
        widget=forms.TextInput(attrs={'class': 'form-control'})
    )
    
    # MultipleChoiceField with CheckboxSelectMultiple creates the checkboxes
    styles = forms.MultipleChoiceField(
        choices=STYLE_CHOICES, 
        widget=forms.CheckboxSelectMultiple,
        required=False, # It's okay if user selects nothing
        label=""
    )
    
    # ChoiceField with RadioSelect creates the radio buttons
    color = forms.ChoiceField(
        choices=COLOR_CHOICES, 
        widget=forms.RadioSelect,
        initial='red', # Default selection
        label=""
    )
    
    message = forms.CharField(
        label="Enter Message", 
        widget=forms.Textarea(attrs={'rows': 4, 'cols': 40})
    )