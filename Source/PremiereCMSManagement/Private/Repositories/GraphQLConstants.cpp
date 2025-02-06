
#include "Repositories/GraphQLConstants.h"

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
			id
			givenName
			familyName
			artisticName
		  } 
		}
	)");

const FString GQLPerformanceData = TEXT(R"(
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
