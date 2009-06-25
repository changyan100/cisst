/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: mtsDeviceInterfaceProxyServer.h 142 2009-03-11 23:02:34Z mjung5 $

  Author(s):  Min Yang Jung
  Created on: 2009-04-24

  (C) Copyright 2009 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

#ifndef _mtsDeviceInterfaceProxyServer_h
#define _mtsDeviceInterfaceProxyServer_h

#include <cisstCommon/cmnDeSerializer.h>
#include <cisstMultiTask/mtsDeviceInterface.h>
#include <cisstMultiTask/mtsDeviceInterfaceProxy.h>
#include <cisstMultiTask/mtsProxyBaseServer.h>
#include <cisstMultiTask/mtsFunctionVoid.h>
#include <cisstMultiTask/mtsFunctionReadOrWrite.h>
#include <cisstMultiTask/mtsFunctionQualifiedReadOrWrite.h>
#include <cisstMultiTask/mtsCommandVoidProxy.h>
#include <cisstMultiTask/mtsCommandWriteProxy.h>

#include <cisstMultiTask/mtsExport.h>

#include <string>

/*!
  \ingroup cisstMultiTask

  TODO: add class summary here
*/
class mtsTask;

class CISST_EXPORT mtsDeviceInterfaceProxyServer : public mtsProxyBaseServer<mtsTask> {
    
    CMN_DECLARE_SERVICES(CMN_NO_DYNAMIC_CREATION, CMN_LOG_LOD_RUN_ERROR);

public:
    mtsDeviceInterfaceProxyServer(const std::string& adapterName,
                                  const std::string& endpointInfo,
                                  const std::string& communicatorID);
    ~mtsDeviceInterfaceProxyServer();
    
    void SetConnectedTask(mtsTask * task) { ConnectedTask = task; }

protected:
    /*! Typedef for client proxy. */
    typedef mtsDeviceInterfaceProxy::DeviceInterfaceClientPrx DeviceInterfaceClientProxyType;

    /*! Typedef for base type. */
    typedef mtsProxyBaseServer<mtsTask> BaseType;

    /*! Pointer to the task connected. */
    mtsTask * ConnectedTask;

    /*! Connected client object. */
    DeviceInterfaceClientProxyType ConnectedClient;

    //-------------------------------------------------------------------------
    //  Proxy Implementation
    //-------------------------------------------------------------------------
    /*! Create a servant which serves TaskManager clients. */
    Ice::ObjectPtr CreateServant() {
        Sender = new DeviceInterfaceServerI(IceCommunicator, Logger, this);
        return Sender;
    }
    
    /*! Entry point to run a proxy. */
    void Start(mtsTask * callingTask);

    /*! Start a send thread and wait for shutdown (blocking call). */
    void StartServer();

    /*! Thread runner */
    static void Runner(ThreadArguments<mtsTask> * arguments);

    /*! Clean up thread-related resources. */
    void OnThreadEnd();
    
    /*! Definitions for send thread */
    class DeviceInterfaceServerI;
    typedef IceUtil::Handle<DeviceInterfaceServerI> DeviceInterfaceServerIPtr;
    DeviceInterfaceServerIPtr Sender;

    /*! Resource clean-up */
    void OnClose();

    //-------------------------------------------------------------------------
    //  Serialization and Deserialization
    //-------------------------------------------------------------------------
    /*! Buffers for serialization and deserialization. */
    std::stringstream SerializationBuffer;
    std::stringstream DeSerializationBuffer;

    /*! Serializer and DeSerializer. */
    cmnSerializer * Serializer;
    cmnDeSerializer * DeSerializer;

    //-------------------------------------------------------------------------
    //  Function Proxy and Event Proxy
    //-------------------------------------------------------------------------
    /*! Function proxy */
    typedef cmnNamedMap<mtsFunctionVoid>  FunctionVoidProxyMapType;
    typedef cmnNamedMap<mtsFunctionWrite> FunctionWriteProxyMapType;
    typedef cmnNamedMap<mtsFunctionRead>  FunctionReadProxyMapType;
    typedef cmnNamedMap<mtsFunctionQualifiedRead> FunctionQualifiedReadProxyMapType;
    FunctionVoidProxyMapType FunctionVoidProxyMap;
    FunctionWriteProxyMapType FunctionWriteProxyMap;
    FunctionReadProxyMapType FunctionReadProxyMap;
    FunctionQualifiedReadProxyMapType FunctionQualifiedReadProxyMap;

    /*! Event proxy */
    typedef cmnNamedMap<mtsCommandVoidProxy>  EventHandlerVoidMapType;
    typedef cmnNamedMap<mtsCommandWriteProxy> EventHandlerWriteMapType;
    EventHandlerVoidMapType  EventHandlerVoidMap;
    EventHandlerWriteMapType EventHandlerWriteMap;

    //-------------------------------------------------------------------------
    //  Processing Methods
    //-------------------------------------------------------------------------
    mtsProvidedInterface * GetProvidedInterface(
        const std::string resourceDeviceName, const std::string providedInterfaceName) const;

    bool PopulateRequiredInterfaceProxy(mtsRequiredInterface * requiredInterfaceProxy, 
                                        mtsProvidedInterface * providedInterface);

    /*! Get pointers to the function proxies created at 
        mtsDeviceInterfaceProxyServer::PopulateRequiredInterfaceProxy(). */        
    //bool GetFunctionPointers(const std::string & serverTaskProxyName, 
    //                         const std::string & providedInterfaceProxyName);
    void GetFunctionPointers(mtsDeviceInterfaceProxy::FunctionProxySet & functionProxySet);

    //-------------------------------------------------------------------------
    //  Methods to Receive and Process Events
    //-------------------------------------------------------------------------
    /*! When a new client connects, add it to the client management list. */
    void ReceiveAddClient(const DeviceInterfaceClientProxyType & clientProxy);

    /*! Update the information of all tasks. */
    const bool ReceiveGetProvidedInterfaces(
        ::mtsDeviceInterfaceProxy::ProvidedInterfaceSequence & providedInterfaces);

    /*! Connect at server side. 
        This method creates a client task proxy (mtsDeviceProxy) and a required
        interface proxy (mtsRequiredInterface) at server side. */
    bool ReceiveConnectServerSide(
        const std::string & userTaskName, const std::string & requiredInterfaceName,
        const std::string & resourceTaskName, const std::string & providedInterfaceName);

    /*! Update command id. */
    void ReceiveGetCommandId(mtsDeviceInterfaceProxy::FunctionProxySet & functionProxies);

    /*! Execute actual command objects. */
    void ReceiveExecuteCommandVoid(const int commandId) const;
    void ReceiveExecuteCommandWriteSerialized(const int commandId, const std::string argument);
    void ReceiveExecuteCommandReadSerialized(const int commandId, std::string & argument);
    void ReceiveExecuteCommandQualifiedReadSerialized(const int commandId, const std::string argument1, std::string & argument2);

    //-------------------------------------------------------------------------
    //  Methods to Send Events
    //-------------------------------------------------------------------------
    //void SendUpdateCommandId(const mtsDeviceInterfaceProxy::FunctionProxySet & functionProxySet);

    //-------------------------------------------------------------------------
    //  Definition by mtsDeviceInterfaceProxy.ice
    //-------------------------------------------------------------------------
    class DeviceInterfaceServerI : public mtsDeviceInterfaceProxy::DeviceInterfaceServer,
                                   public IceUtil::Monitor<IceUtil::Mutex> 
    {
    private:
        Ice::CommunicatorPtr Communicator;
        bool Runnable;
        IceUtil::ThreadPtr Sender;
        Ice::LoggerPtr Logger;
        mtsDeviceInterfaceProxyServer * DeviceInterfaceServer;

    public:
        DeviceInterfaceServerI(const Ice::CommunicatorPtr& communicator, 
                             const Ice::LoggerPtr& logger,
                             mtsDeviceInterfaceProxyServer * DeviceInterfaceServer);

        void Start();
        void Run();
        void Destroy();

        void AddClient(const ::Ice::Identity&, const ::Ice::Current&);
        bool GetProvidedInterfaces(::mtsDeviceInterfaceProxy::ProvidedInterfaceSequence&, 
                                   const ::Ice::Current&) const;
        bool ConnectServerSide(
            const std::string & userTaskName, const std::string & requiredInterfaceName,
            const std::string & resourceTaskName, const std::string & providedInterfaceName,
            const ::Ice::Current&);
        void GetCommandId(::mtsDeviceInterfaceProxy::FunctionProxySet&, const ::Ice::Current&) const;

        void ExecuteCommandVoid(::Ice::Int, const ::Ice::Current&);
        void ExecuteCommandWriteSerialized(::Ice::Int, const ::std::string&, const ::Ice::Current&);
        void ExecuteCommandReadSerialized(::Ice::Int, ::std::string&, const ::Ice::Current&);
        void ExecuteCommandQualifiedReadSerialized(::Ice::Int, const ::std::string&, ::std::string&, const ::Ice::Current&);

    };
};

CMN_DECLARE_SERVICES_INSTANTIATION(mtsDeviceInterfaceProxyServer)

#endif // _mtsDeviceInterfaceProxyServer_h

