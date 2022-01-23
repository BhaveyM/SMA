
chrome.tabs.onUpdated.addListener((tabId, change, tab)=> {
	if (new RegExp("web.whatsapp.com").test(tab.url) && change.status == "complete") {
		chrome.tabs.sendMessage(tabId, {d: true});
	}
});