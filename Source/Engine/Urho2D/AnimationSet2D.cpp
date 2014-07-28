//
// Copyright (c) 2008-2014 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "Precompiled.h"
#include "Animation2D.h"
#include "AnimationSet2D.h"
#include "Context.h"
#include "FileSystem.h"
#include "Log.h"
#include "ResourceCache.h"
#include "Sprite2D.h"
#include "SpriteSheet2D.h"
#include "XMLFile.h"

#include "DebugNew.h"

namespace Urho3D
{

AnimationSet2D::AnimationSet2D(Context* context) :
    Resource(context)
{
}

AnimationSet2D::~AnimationSet2D()
{
}

void AnimationSet2D::RegisterObject(Context* context)
{
    context->RegisterFactory<AnimationSet2D>();
}

bool AnimationSet2D::BeginLoad(Deserializer& source)
{
    loadXMLFile_ = new XMLFile(context_);
    if (!loadXMLFile_->Load(source))
    {
        LOGERROR("Load XML failed " + source.GetName());
        loadXMLFile_.Reset();
        return false;
    }

    XMLElement rootElem = loadXMLFile_->GetRoot("spriter_data");
    if (!rootElem)
    {
        LOGERROR("Invalid spriter file " + source.GetName());
        loadXMLFile_.Reset();
        return false;
    }
    
    // When async loading, preprocess folders for spritesheet / sprite files and request them for background loading
    if (GetAsyncLoadState() == ASYNC_LOADING)
    {
        if (!LoadFolders(rootElem))
        {
            loadXMLFile_.Reset();
            return false;
        }
    }

    return true;
}

bool AnimationSet2D::EndLoad()
{
    // Actually load the folders and animations now
    if (!loadXMLFile_)
        return false;

    XMLElement rootElem = loadXMLFile_->GetRoot("spriter_data");
    if (!LoadFolders(rootElem))
    {
        loadXMLFile_.Reset();
        return false;
    }

    XMLElement entityElem = rootElem.GetChild("entity");
    if (!entityElem)
    {
        LOGERROR("Could not find entity");
        loadXMLFile_.Reset();
        return false;
    }
    
    for (XMLElement animationElem = entityElem.GetChild("animation"); animationElem; animationElem = animationElem.GetNext("animation"))
    {
        if (!LoadAnimation(animationElem))
        {
            loadXMLFile_.Reset();
            return false;
        }
    }

    loadXMLFile_.Reset();
    return true;
}

unsigned AnimationSet2D::GetNumAnimations() const
{
    return animations_.Size();
}

Animation2D* AnimationSet2D::GetAnimation(unsigned index) const
{
    if (index < animations_.Size())
        return animations_[index];
    return 0;
}

Animation2D* AnimationSet2D::GetAnimation(const String& name) const
{
    for (unsigned i = 0; i < animations_.Size(); ++i)
    {
        if (animations_[i]->GetName() == name)
            return animations_[i];
    }
    return 0;
}

bool AnimationSet2D::LoadFolders(const XMLElement& rootElem)
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();

    bool async = GetAsyncLoadState() == ASYNC_LOADING;

    String parentPath = GetParentPath(GetName());
    String spriteSheetFilePath = parentPath + GetFileName(GetName()) + ".xml";
    SpriteSheet2D* spriteSheet = 0;
    bool hasSpriteSheet = false;

    // When async loading, request the sprite sheet for background loading but do not actually get it
    if (!async)
        spriteSheet = cache->GetResource<SpriteSheet2D>(spriteSheetFilePath, false);
    else
    {
        hasSpriteSheet = cache->Exists(spriteSheetFilePath);
        if (hasSpriteSheet)
            cache->BackgroundLoadResource<SpriteSheet2D>(spriteSheetFilePath, false, this);
    }

    for (XMLElement folderElem = rootElem.GetChild("folder"); folderElem; folderElem = folderElem.GetNext("folder"))
    {
        unsigned folderId = folderElem.GetUInt("id");

        for (XMLElement fileElem = folderElem.GetChild("file"); fileElem; fileElem = fileElem.GetNext("file"))
        {
            unsigned fileId = fileElem.GetUInt("id");
            String fileName = fileElem.GetAttribute("name");

            // When async loading, request the sprites for background loading but do not actually get them
            if (!async)
            {
                SharedPtr<Sprite2D> sprite;
                
                if (spriteSheet)
                    sprite = spriteSheet->GetSprite(GetFileName(fileName));
                else
                    sprite = (cache->GetResource<Sprite2D>(parentPath + fileName));

                if (!sprite)
                {
                    LOGERROR("Could not load sprite " + fileName);
                    return false;
                }

                Vector2 hotSpot(0.0f, 1.0f);
                if (fileElem.HasAttribute("pivot_x"))
                    hotSpot.x_ = fileElem.GetFloat("pivot_x");
                if (fileElem.HasAttribute("pivot_y"))
                    hotSpot.y_ = fileElem.GetFloat("pivot_y");

                // If sprite is trimmed, recalculate hot spot
                const IntVector2& offset = sprite->GetOffset();
                if (offset != IntVector2::ZERO)
                {
                    int width = fileElem.GetInt("width");
                    int height = fileElem.GetInt("height");

                    float pivotX = width * hotSpot.x_;
                    float pivotY = height * (1.0f - hotSpot.y_);

                    const IntRect& rectangle = sprite->GetRectangle();
                    hotSpot.x_ = (offset.x_ + pivotX) / rectangle.Width();
                    hotSpot.y_ = 1.0f - (offset.y_ + pivotY) / rectangle.Height();
                }

                sprite->SetHotSpot(hotSpot);

                sprites_[(folderId << 16) + fileId] = sprite;
            }
            else if (!hasSpriteSheet)
                cache->BackgroundLoadResource<Sprite2D>(parentPath + fileName, true, this);
        }
    }

    return true;
}

Sprite2D* AnimationSet2D::GetSprite(unsigned folderId, unsigned fileId) const
{
    unsigned key = (folderId << 16) + fileId;
    HashMap<unsigned, SharedPtr<Sprite2D> >::ConstIterator i = sprites_.Find(key);
    if (i != sprites_.End())
        return i->second_;
    return 0;
}

bool AnimationSet2D::LoadAnimation(const XMLElement& animationElem)
{
    SharedPtr<Animation2D> animation(new Animation2D(this));
    
    String name = animationElem.GetAttribute("name");
    animation->SetName(name);

    float length = animationElem.GetFloat("length") * 0.001f;
    animation->SetLength(length);

    bool looped = true;
    if (animationElem.HasAttribute("looping"))
        looped = animationElem.GetBool("looping");
    animation->SetLooped(looped);

    // Load timelines
    for (XMLElement timelineElem = animationElem.GetChild("timeline"); timelineElem; timelineElem = timelineElem.GetNext("timeline"))
    {
        Timeline2D timeline;
        timeline.name_ = timelineElem.GetAttribute("name");
        if (timelineElem.GetAttribute("object_type") == "bone")
            timeline.type_ = OT_BONE;
        else
            timeline.type_ = OT_SPRITE;

        for (XMLElement keyElem = timelineElem.GetChild("key"); keyElem; keyElem = keyElem.GetNext("key"))
        {
            TimelineKey2D key;
            key.time_ = keyElem.GetFloat("time") * 0.001f;
            key.spin_ = 1;
            if (keyElem.HasAttribute("spin"))
                key.spin_ = keyElem.GetInt("spin");

            XMLElement childElem = keyElem.GetChild();

            Vector2 position;
            position.x_ = childElem.GetFloat("x");
            position.y_ = childElem.GetFloat("y");

            float angle = childElem.GetFloat("angle");

            Vector2 scale(Vector2::ONE);
            if (childElem.HasAttribute("scale_x"))
                scale.x_ = childElem.GetFloat("scale_x");

            if (childElem.HasAttribute("scale_y"))
                scale.y_ = childElem.GetFloat("scale_y");

            key.transform_ = Transform2D(position, angle, scale);

            if (timeline.type_ == OT_SPRITE)
            {
                int folder = childElem.GetUInt("folder");
                int file = childElem.GetUInt("file");
                key.sprite_ = GetSprite(folder, file);
                if (!key.sprite_)
                {
                    LOGERROR("Could not find sprite");
                    return false;
                }

                if (childElem.HasAttribute("pivot_x"))
                    key.hotSpot_.x_ = childElem.GetFloat("pivot_x");
                else
                    key.hotSpot_.x_ = key.sprite_->GetHotSpot().x_;

                if (childElem.HasAttribute("pivot_y"))
                    key.hotSpot_.y_ = childElem.GetFloat("pivot_y");
                else
                    key.hotSpot_.y_ = key.sprite_->GetHotSpot().y_;

                if (childElem.HasAttribute("a"))
                    key.alpha_ = childElem.GetFloat("a");
            }

            timeline.timelineKeys_.Push(key);
        }

        // Add end key for looped animation
        if (looped && timeline.timelineKeys_.Back().time_ != length)
        {
            TimelineKey2D key = timeline.timelineKeys_.Front();
            key.time_ = length;
            timeline.timelineKeys_.Push(key);
        }

        animation->AddTimeline(timeline);
    }

    // Load main line
    XMLElement mainlineElem = animationElem.GetChild("mainline");
    for (XMLElement keyElem = mainlineElem.GetChild("key"); keyElem; keyElem = keyElem.GetNext("key"))
    {
        MainlineKey2D mainlineKey;
        int id = keyElem.GetInt("id");
        mainlineKey.time_ = keyElem.GetFloat("time") * 0.001f;

        for (XMLElement refElem = keyElem.GetChild(); refElem; refElem = refElem.GetNext())
        {
            Reference2D ref;
            
            int refId = refElem.GetInt("id");
            if (refElem.GetName() == "bone_ref")
                ref.type_ = OT_BONE;
            else
                ref.type_ = OT_SPRITE;

            ref.timeline_ = refElem.GetInt("timeline");

            if (refElem.HasAttribute("parent"))
            {
                int parent = refElem.GetInt("parent");
                int parentTimeline = mainlineKey.references_[parent].timeline_;
                animation->SetTimelineParent(ref.timeline_, parentTimeline);
            }
            
            if (refElem.GetName() == "object_ref")
                ref.zIndex_ = refElem.GetInt("z_index");

            mainlineKey.references_.Push(ref);
        }

        animation->AddMainlineKey(mainlineKey);
    }

    animations_.Push(animation);

    return true;
}

}
