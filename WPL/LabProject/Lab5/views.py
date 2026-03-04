from django.shortcuts import render
from django.http import HttpResponse
from datetime import datetime, date
import random
import string
from .forms import TextFormatterForm

def student_view(request):
    context = {}
    if request.method == 'POST':
        name = request.POST.get('name')
        eng = int(request.POST.get('english', 0))
        phy = int(request.POST.get('physics', 0))
        chem = int(request.POST.get('chemistry', 0))
        
        total = eng + phy + chem
        percentage = round((total / 300) * 100, 2)
        
        details_str = (
            f"Name: {name}\n"
            f"Address: {request.POST.get('address')}\n"
            f"Email: {request.POST.get('email')}\n"
            f"Marks: Eng({eng}), Phy({phy}), Chem({chem})"
        )
        
        context['details_log'] = details_str
        context['percentage'] = percentage

    return render(request, 'q1_t.html', context)

def employee_view(request):
    result = None
    
    if request.method == 'POST':
        doj_str = request.POST.get('doj')
        
        if doj_str:
            doj = datetime.strptime(doj_str, "%Y-%m-%d").date()
            today = date.today()
            
            delta = today - doj
            years_experience = delta.days / 365.25
            
            if years_experience > 5:
                result = "YES"
            else:
                result = "NO"

    return render(request, 'q2_t.html', {'result': result})

def generate_captcha(length=5):
    return ''.join(random.choices(string.ascii_uppercase + string.digits, k=length))


def captcha_view(request):
    result = None
    disabled = False

    if request.method == 'GET':
        request.session['captcha_attempts'] = 0
        request.session['captcha_value'] = generate_captcha()

    attempts = request.session.get('captcha_attempts', 0)

    if request.method == 'POST':
        if attempts >= 3:
            disabled = True
            result = "Too many failed attempts. CAPTCHA disabled."
        else:
            user_input = request.POST.get('captcha_input', '').strip().upper()
            captcha_value = request.session.get('captcha_value')

            if user_input == captcha_value:
                result = "CAPTCHA validation successful!"
                request.session['captcha_attempts'] = 0
                request.session['captcha_value'] = generate_captcha()
            else:
                attempts += 1
                request.session['captcha_attempts'] = attempts
                request.session['captcha_value'] = generate_captcha()
                result = "CAPTCHA validation failed."

        if attempts >= 3:
            disabled = True

    return render(request, 'q3_t.html', {
        'result': result,
        'captcha': request.session['captcha_value'],
        'disabled': disabled,
        'attempts': attempts
    })


def forms_view(request):
    result_html = None
    css_style = ""
    
    if request.method == 'POST':
        form = TextFormatterForm(request.POST)
        if form.is_valid():
            name = form.cleaned_data['name']
            message = form.cleaned_data['message']
            styles = form.cleaned_data['styles']
            color = form.cleaned_data['color']

            # Build CSS string based on selection
            style_list = []
            
            # Color logic
            style_list.append(f"color: {color};")
            
            # Style logic
            if 'bold' in styles:
                style_list.append("font-weight: bold;")
            if 'italic' in styles:
                style_list.append("font-style: italic;")
            if 'underline' in styles:
                style_list.append("text-decoration: underline;")
            
            css_style = " ".join(style_list)
            
            # Concatenate for display (matching the screenshot format)
            result_html = f"Name: {name} <br> Message: {message}"
            
    else:
        form = TextFormatterForm()

    return render(request, 'form_t.html', {
        'form': form,
        'result': result_html,
        'css_style': css_style
    })