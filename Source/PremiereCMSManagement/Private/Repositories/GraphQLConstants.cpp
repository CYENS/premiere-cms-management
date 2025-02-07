
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

const FString GQLSessionFragment = TEXT(R"(
fragment sessionFragment on Session {
	id
	eosSessionId
  }
)");

const FString GQLAvatarFragment = TEXT(R"(
fragment avatarFragment on Avatar {
	id
	name
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

const FString GQLSessionFragments = FString::Printf(TEXT(R"(
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
*GQLUserFragment,
*GQLSessionFragment,
*GQLAvatarFragment,
*GQLFaceDataFragment,
*GQLAudioDataFragment,
*GQLLightDataFragment,
*GQLPropMotionDataFragment,
*GQLAvatarMotionDataFragment
);

const FString GQLUsdSceneFragments = FString::Printf(TEXT(R"(
	%s
	%s
)")
,
*GQLUserFragment,
*GQLPerformanceFragment
);

const FString GQLUsdScene = FString::Printf(TEXT(R"(
    id
    pCloudFileId
    fileUrl
    title
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
)"));

const FString GQLSession = TEXT(R"(
    id
    eosSessionId
    title
    state
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
      id
      streamingUrl
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
)");

const FString GQLPerformanceFragments = FString::Printf(TEXT(R"(
	%s
	%s
)")
,
*GQLPersonFragment,
*GQLUserFragment
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
		  id
		  eosSessionId
		  title
		  state
		  owner {
			...userFragment
		  }
		}
		avatars {
		  id
		  name
		}
	)");
