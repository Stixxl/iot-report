FROM python:3


COPY requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt

COPY consumer2.py /

CMD [ "python", "./consumer2.py" ]
