let hidefiles = document.querySelector("#files");
let hideImages = document.querySelector("#himgs");
let unhideImages = document.querySelector("#uimgs");
let ext = '';
let hide = document.querySelector("#en"); // button element <button>...</button>
let unhide = document.querySelector("#de"); // button element <button>...</button>


hide.addEventListener('click', async ()=> {
	let formData = new FormData();
	formData.append('hide', hidefiles.files[0]);
	formData.append('img', hideImages.files[0]);
	ext = hidefiles.value.split("\\").pop().split(".").pop();
	hidefiles.value = '', hideImages.value = '';

	let res = await fetch("/hide", {
		method: 'post',
		headers: {
			//'Content-Type': 'application/json'
			'enctype': "multipart/form-data"
		},
		body: formData
	});
	let json = await res.blob();

	const imageObjectURL = URL.createObjectURL(json);
	let a = document.createElement('a');
	a.setAttribute("download", "hidden.png");
	a.setAttribute("href", imageObjectURL);
	a.click();
	a.remove();
});

unhide.addEventListener('click', async ()=> {
	let formData = new FormData();
	formData.append('img', unhideImages.files[0]);
	unhideImages.value = '';

	let res = await fetch("/unhide", {
		method: 'post',
		headers: {
			//'Content-Type': 'application/json'
			'enctype': "multipart/form-data"
		},
		body: formData
	});
	let json = await res.blob();
	const imageObjectURL = URL.createObjectURL(json);
	
	let a = document.createElement('a');
	a.setAttribute("download", "decoded."+ext);
	
	a.setAttribute("href", imageObjectURL);
	a.click();
	a.remove();
});
