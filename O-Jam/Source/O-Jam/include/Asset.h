#pragma once

#include <NodeContent.h>
#include <Texture.h>
#include <TextureSampler.h>
#include <json\json.h>

class Asset abstract : public NodeContent{
public:
	std::string id;

	Asset(Json::Value _json);

	static Asset * getAsset(Json::Value _json);
};

class AssetTexture : public Asset{
public:
	float width;
	float height;
	Texture * texture;

	AssetTexture(Json::Value _json);
	~AssetTexture();
};