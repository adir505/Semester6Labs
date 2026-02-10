from django.urls import path
from . import views

urlpatterns = [
    path('lab5/q1/', views.student_view, name='student_view'),
    path('lab5/q2/', views.employee_view, name='employee_view'),
]