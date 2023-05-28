import { Module } from '@nestjs/common';
import { AppController } from './app.controller';
import { AppService } from './app.service';

import { MongooseModule } from '@nestjs/mongoose';

import { Path, PathSchema } from './schemas/path.schema';
import { Mongo } from './config';

@Module({
  imports: [MongooseModule.forRoot(Mongo.URI),
  MongooseModule.forFeature([{name:Path.name, schema: PathSchema}])],
  controllers: [AppController],
  providers: [AppService ],
})
export class AppModule {}
