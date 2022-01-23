let lists = document.querySelector("#list");
function display() {
	lists.innerHTML = "";
	chrome.storage.local.get(['tiktokData'], (result)=> {
		//console.log(result)
		result.tiktokData.forEach((z, i)=> {
			let tempEle = document.createElement("span");
			tempEle.innerHTML = `<img src="${z.i}" /><div class="navs"><a class="items" href="${z.l}" target="_blank">${z.l.slice(z.l.indexOf("@")+1, z.l.indexOf("video")-1)}</a><a href="${z.i}" target="_blank">download</a></div>`;
			lists.append(tempEle);
		});
	});
}
window.onload = ()=> {
	display();
}
chrome.runtime.onMessage.addListener(function(msg, sender, sendResponse) {
	if (msg.stored) {
		display();
	}
});
