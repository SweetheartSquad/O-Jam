#pragma once

#include <Asset.h>

Asset::Asset(Json::Value _json) :
	id(_json.get("id", "NO_ID").asString())
{

}

AssetTexture::AssetTexture(Json::Value _json) :
	Asset(_json),
	width(_json.get("width", 256).asInt()),
	height(_json.get("height", 256).asInt()),
	texture(nullptr)
{
	std::string src = _json.get("src", "NO_TEXTURE").asString();
	if(src == "NO_TEXTURE"){
		src = "../assets/engine basics/img_cheryl.jpg";
	}else{
		src = "../assets/textures/" + src;
	}
	texture = new Texture(src, width, height, true, false);
	texture->load();
}
AssetTexture::~AssetTexture(){
	delete texture;
}


Asset * Asset::getAsset(Json::Value _json){
	Asset * res = nullptr;
	std::string type = _json.get("type", "NO_TYPE").asString();
	// create a different type of Trigger depending on the value of type
	if(type == "texture"){
		res = new AssetTexture(_json);
	}else{
		throw "invalid trigger type";
	}
	return res;
}