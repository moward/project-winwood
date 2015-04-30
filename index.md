---
layout: default
title: Home
---


#Project Winwood

##Blog Posts

{% for post in site.posts %}
- [{{ post.title }}]({{ post.url }}), {{ post.date | date_to_string }}{% endfor %}

##About Project Winwood

Project Winwood is a final project for [ESE 350 - Introduction to Embedded Systems](http://www.seas.upenn.edu/~ese350/) at the University of Pennsylvania for the Spring of 2015. It is being developed by two sophomore Computer Engineering majors, Matt Howard and Chris Champaigne. The project is being mentored by [Professor Rahul Mangharam](http://www.seas.upenn.edu/~rahulm/), Michelle Liu, and [Steven Xing](http://www.seas.upenn.edu/cmpe/student-profiles/xing.php).