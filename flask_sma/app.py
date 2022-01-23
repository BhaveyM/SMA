from flask import Flask, url_for, render_template, request, send_from_directory, abort
from werkzeug.utils import secure_filename
import steg
import os

app = Flask(__name__)

@app.route("/")
def homepage():
    url_for('static', filename='styles/style.css')
    url_for('static', filename='src/hideScript.js')
    return render_template('index.html')

@app.route("/hide", methods=["POST"])
def hidepage():
    print("hide")
    global file_size
    global ext
    hidefile = request.files["hide"]
    imgfile = request.files["img"]
    
    hidefile.save(f"uploads/{secure_filename(hidefile.filename)}")
    imgfile.save(f"uploads/{secure_filename(imgfile.filename)}")

    split_tup = os.path.splitext("uploads/"+secure_filename(hidefile.filename))
    ext = split_tup[1]
    if(ext == '.txt'):
        txt = open("uploads/"+secure_filename(hidefile.filename), "r").read()
        txt += "\0"
        file_size = len(txt)
    else:
        file_size = os.path.getsize("uploads/"+secure_filename(hidefile.filename))

    steg.hide("uploads/"+secure_filename(hidefile.filename), "uploads/"+secure_filename(imgfile.filename), ext, file_size)

    if os.path.exists("uploads/decoded.txt") or os.path.exists("uploads/"+secure_filename(hidefile.filename)) or os.path.exists("uploads/"+secure_filename(imgfile.filename)):
        os.remove("uploads/"+secure_filename(hidefile.filename))
        os.remove("uploads/"+secure_filename(imgfile.filename))
        
    try:
        return send_from_directory("uploads/", "hidden.png", as_attachment=True)
    except FileNotFoundError:
        abort(404)

@app.route("/unhide", methods=["POST"])
def unhidepage():
    print("unhide")
    imgfile = request.files["img"]

    imgfile.save(f"uploads/{secure_filename(imgfile.filename)}")
    steg.unhide("uploads/"+secure_filename(imgfile.filename), ext, file_size)

    if os.path.exists("uploads/hidden.png"):
        os.remove("uploads/hidden.png")

    try:
        return send_from_directory("uploads", "decoded"+ext, as_attachment=True)
        os.remove("uploads/decoded"+ext)
    except FileNotFoundError:
        abort(404)