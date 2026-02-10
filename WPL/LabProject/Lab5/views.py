from django.shortcuts import render
from django.http import HttpResponse
from datetime import datetime, date

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