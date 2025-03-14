
#include "Repositories/GraphQLConstants.h"

const FString GQLPersonFragment = TEXT(R"(
fragment personFragment on Person {
	id
	givenName
	familyName
	artisticName
}
)");

const FString GQLUserFragment = TEXT(R"(
	fragment userFragment on User {
	  id
	  name
	  email
	  eosId
	  userRole
	  isAdmin
	  isSuperAdmin
	  createdAt
	  person {
		...personFragment
	  } 
	}
)");

const FString GQLPerformanceFragment = TEXT(R"(
fragment performanceFragment on Performance {
	id
	title
	about
	owner {
		...userFragment
	}
}
)");

const FString GQLUsdXrLiveFragment = TEXT(R"(
fragment xrLiveFragment on XR_Live {
	id
	about
	approved
	dataStatus
	dataUrl
	performanceOrRehearsal
	quality
	streamingUrl
}
)");

const FString GQLUsdSceneFragment = TEXT(R"(
fragment usdSceneFragment on UsdScene {
    id
    title
    fileUrl
    uploadAt
    pCloudFileId
    public
    template
    owner {
      ...userFragment  
    }
}
)");

const FString GQLSessionFragment = TEXT(R"(
fragment sessionFragment on Session {
	id
	eosSessionId
	title
	state {
		...sessionStateFragment
	}
	owner {
		...userFragment
	}
    scene {
	  ...usdSceneFragment
    }
    xrLive {
	  ...xrLiveFragment
    }
    performance {
      ...performanceFragment
    }
	castAvatars {
	  ...sessionCastFragment
	}
  }
)");

const FString GQLAvatarFragment = TEXT(R"(
fragment avatarFragment on Avatar {
	id
	name
	assetId
  }
)");

const FString GQLFaceDataFragment = TEXT(R"(
fragment faceDataFragment on FaceData {
	id
	pCloudFileId
	fileUrl
	session {
		...sessionFragment
	}
	avatar {
		...avatarFragment
	}
}
)");

const FString GQLAudioDataFragment = TEXT(R"(
fragment audioDataFragment on AudioData {
	id
	pCloudFileId
	fileUrl
	avatar {
		...avatarFragment  
	  }
	session {
		...sessionFragment
	  }
}
)");

const FString GQLLightDataFragment = TEXT(R"(
fragment lightDataFragment on LightData {
	id
	pCloudFileId
	fileUrl
	lightId
	initialPositionX
	initialPositionY
	initialPositionZ
	initialRotationX
	initialRotationY
	initialRotationZ
	lightType
	lightCharacteristicsJson
	session {
		...sessionFragment  
	}
}
)");

const FString GQLPropMotionDataFragment = TEXT(R"(
fragment propMotionFragment on PropMotionData {
	id
	pCloudFileId
	fileUrl
	initialPositionX
	initialPositionY
	initialPositionZ
	initialRotationX
	initialRotationY
	initialRotationZ
  }
)");

const FString GQLAvatarMotionDataFragment = TEXT(R"(
fragment motionFragment on AvatarMotionData {
	id
	pCloudFileId
	fileUrl
	initialPositionX
	initialPositionY
	initialPositionZ
	initialRotationX
	initialRotationY
	initialRotationZ
	avatar {
		...avatarFragment
	}
}
)");

const FString GQLUserFragments = FString::Printf(TEXT(R"(
	%s
)"),
    *GQLPersonFragment
);

const FString GQLUser = FString::Printf(TEXT(R"(
    id
    name
    email
    eosId
    userRole
    isAdmin
    isSuperAdmin
    createdAt
    person {
		...personFragment
    }
)"));

const FString GQLSessionFragmentWithoutCastAvatars = TEXT(R"(
fragment sessionFragmentWithoutCastAvatars on Session {
	id
	eosSessionId
	title
	state {
		...sessionStateFragment
	}
	owner {
		...userFragment
	}
    scene {
	  ...usdSceneFragment
    }
    xrLive {
	  ...xrLiveFragment
    }
    performance {
      ...performanceFragment
    }
}
)");

const FString GQLSessionCastFragment = TEXT(R"(
fragment sessionCastFragment on SessionCast {
  id
  label
  avatar {
	...avatarFragment
  }
  user {
	...userFragment
  }
  session {
	...sessionFragmentWithoutCastAvatars
  }
}
)");

const FString GQLSessionStateFragment = TEXT(R"(
	fragment sessionStateFragment on SessionState {
		id
		name
	}
)");

const FString GQLSessionFragments = FString::Printf(TEXT(R"(
	%s
	%s
	%s
	%s
	%s
	%s
	%s
	%s
	%s
	%s
	%s
	%s
	%s
	%s
	%s
)"),
*GQLUsdXrLiveFragment,
*GQLUsdSceneFragment,
*GQLPersonFragment,
*GQLUserFragment,
*GQLSessionStateFragment,
*GQLSessionFragment,
*GQLPerformanceFragment,
*GQLAvatarFragment,
*GQLFaceDataFragment,
*GQLAudioDataFragment,
*GQLLightDataFragment,
*GQLPropMotionDataFragment,
*GQLAvatarMotionDataFragment,
*GQLSessionFragmentWithoutCastAvatars,
*GQLSessionCastFragment
);

const FString GQLUsdSceneFragments = FString::Printf(TEXT(R"(
%s
%s
%s
%s
)")
,
*GQLUsdXrLiveFragment,
*GQLPersonFragment,
*GQLUserFragment,
*GQLPerformanceFragment
);

const FString GQLUsdScene = FString::Printf(TEXT(R"(
    id
    title
    pCloudFileId
    fileUrl
    template
    public
    owner {
      ...userFragment  
    }
    members {
      ...userFragment
    }
    performances {
      ...performanceFragment
    }
    xrLive {
	  ...xrLiveFragment
    }
)"));

const FString GQLSession = TEXT(R"(
    id
    eosSessionId
    title
    state {
	  ...sessionStateFragment
	}
    owner {
      ...userFragment
    }
    scene {
      id
      pCloudFileId
      fileUrl
      title
      template
      public
      owner {
        ...userFragment
      }
    }
    xrLive {
	  ...xrLiveFragment
    }
    performance {
      ...performanceFragment
    }
    motionData {
      ...motionFragment
    }
    faceData {
      ...faceDataFragment
    }
    lightData {
      ...lightDataFragment
    }
    audioData {
      ...audioDataFragment
    }
    propMotionData {
      ...propMotionFragment
    }
    attendees {
      ...userFragment  
    }
	castAvatars {
	  ...sessionCastFragment
	}
)");

const FString GQLPerformanceFragments = FString::Printf(TEXT(R"(
	%s
	%s
	%s
	%s
	%s
	%s
	%s
	%s
	%s
	%s
)")
,
*GQLPerformanceFragment,
*GQLUsdXrLiveFragment,
*GQLUsdSceneFragment,
*GQLSessionStateFragment,
*GQLSessionFragment,
*GQLPersonFragment,
*GQLUserFragment,
*GQLAvatarFragment,
*GQLSessionFragmentWithoutCastAvatars,
*GQLSessionCastFragment
);

const FString GQLPerformance = TEXT(R"(
		id
		title
		about
		owner {
		  ...userFragment
		}
		members {
		  ...userFragment
		}
		usdScenes {
		  id
		  pCloudFileId
		  fileUrl
		  title
		  owner {
			...userFragment
		  }
		  template
		  public
		}
		sessions {
			...sessionFragment
		}
		avatars {
			...avatarFragment
		}
	)");


const FString GQLAvatarFragments = FString::Printf(TEXT(R"(
	%s
	%s
	%s
)"),
*GQLPersonFragment,
*GQLUserFragment,
*GQLPerformanceFragment
);

const FString GQLAvatar = TEXT(R"(
id
assetId
name
performances {
	...performanceFragment
}
)");

const FString GQLPersonFragments = FString::Printf(TEXT(R"(
%s
%s
)"),
*GQLPersonFragment,
*GQLUserFragment
);

const FString GQLPerson = TEXT(R"(
id
bio
artisticName
givenName
familyName
user {
	...userFragment
}
)");

const FString GQLSessionCastFragments = FString::Printf(TEXT(R"(
%s
%s
%s
%s
%s
%s
%s
%s
)"),
*GQLUsdXrLiveFragment,
*GQLUsdSceneFragment,
*GQLPersonFragment,
*GQLUserFragment,
*GQLSessionStateFragment,
*GQLPerformanceFragment,
*GQLAvatarFragment,
*GQLSessionFragmentWithoutCastAvatars
);

const FString GQLSessionCast = TEXT(R"(
id
label
avatar {
	...avatarFragment
}
user {
	...userFragment
}
session {
	...sessionFragmentWithoutCastAvatars
})");

const FString GQLSessionStatePrivateInactiveId = TEXT("cm73etiku0000juc7l7iy5xoq");
const FString GQLSessionStatePublicActiveId = TEXT("cm73eju9c00004lmje0o2h47l");
const FString GQLSessionStatePrivateActiveId = TEXT("cm74t2qzv0026juc7cw14fvwj");
const FString GQLSessionStatePublicInactiveId = TEXT("cm74tay4f0027juc7pnuje2ac");

FString GetSessionStateId(const EGQLSessionState& SessionState)
{
	switch (SessionState)
	{
		case EGQLSessionState::PrivateInactive:
			return GQLSessionStatePrivateInactiveId;
		case EGQLSessionState::PublicActive:
			return GQLSessionStatePublicActiveId;
		case EGQLSessionState::PrivateActive:
			return GQLSessionStatePrivateActiveId;
		case EGQLSessionState::PublicInactive:
			return GQLSessionStatePublicInactiveId;
		default: 
			return GQLSessionStatePrivateInactiveId;
	}
}

EGQLSessionState GetSessionStateFromSessionStateId(const FString& SessionStateId)
{
	if (SessionStateId == GQLSessionStatePrivateActiveId)
		return EGQLSessionState::PrivateActive;
	if (SessionStateId == GQLSessionStatePublicActiveId)
		return EGQLSessionState::PublicActive;
	if (SessionStateId == GQLSessionStatePrivateInactiveId)
		return EGQLSessionState::PrivateInactive;
	if (SessionStateId == GQLSessionStatePublicInactiveId)
		return EGQLSessionState::PublicInactive;
	return EGQLSessionState::PrivateInactive;
}
