
PSInterface = GUI.addInterface()

local ppBar = PSInterface:addImage({
	position = {0, 0};
	texture = GUI.getTexture("meleIcon.png");
	size = {100, 120};
})


--[[
coloredPPBar = PPbar:addWidget({
	position = {0, 0};
	--size = {1280, 80};
})

colorlessPPBar = PPbar:addWidget({
	position = {0, 80};
	--size = {1280, 40};
})

professionBar = PSInterface:addWidget({
	position = {0, 120};
	--size = {200, 480};
})

meleIcon = professionBar:addImage({
	position = {40, 0};
	--size = {120, 120};
	texture = GUI.getTexture("out/assets/icons/meleIcon.png");
	
})
rangeIcon = professionBar:addImage({
	position = {40, 120};
	--size = {120, 120};
	texture = GUI.getTexture("out/assets/icons/meleIcon.png")	
})
auraIcon = professionBar:addImage({
	position = {40, 240};
	--size = {120, 120};
	texture = GUI.getTexture("out/assets/icons/meleIcon.png");
	
})
innerFireIcon = professionBar:addImage({
	position = {40, 360};
	--size = {120, 120};
	texture = GUI.getTexture("out/assets/icons/meleIcon.png");
	
})

centerWindow = PSInterface:addWidget({
	position = {200, 120};	
	--size = { 760, 480 };
})

optionList = centerWindow:addScroll({
	position = {40, 40};
	size = {240, 400};
	scrollSpeed = 5;
})

skills = {}

for i = 0, 6 do
	skills[i] = optionList:addWidget({
		position = {0, i * 100};
		--size = { 240, 80 };
	})
end
--]]
